#include "Maze.h"

#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QRandomGenerator>

#include <iostream>
#include <set>
#include <queue>
#include <unordered_map>

Maze::Maze(int width, int height, bool ensurePathFromTopLeftToBottomRightExists, bool markPath, int minLength) : m_width(width), m_height(height), m_cells() {
	std::size_t redoCounter = 0;
	do {
		++redoCounter;
		m_cells.clear();
		for (int w = 0; w < m_width; ++w) {
			for (int h = 0; h < m_height; ++h) {
				m_cells.push_back(Cell::Blocked);
			}
		}

		// Random starting cell
		Position const start(QRandomGenerator::global()->bounded(0, m_width), QRandomGenerator::global()->bounded(0, m_height));
		m_cells.at(coordToIndex(start)) = Cell::Passage;
		std::set<Maze::Position> frontier = getFrontier(start);
		extend(frontier);

		// Enable entry and exit
		m_cells.at(coordToIndex({ 2, 1 })) = Cell::Passage;
		m_cells.at(coordToIndex({ m_width - 2, m_height - 1 })) = Cell::Passage;

		m_path = findPath({ 2,2 }, { m_width - 2, m_height - 2 });
	} while (ensurePathFromTopLeftToBottomRightExists && m_path.empty() || ((minLength > 0) && (m_path.size() < minLength)));

	if (!m_path.empty() && markPath) {
		this->markPath();
	}
}

int Maze::coordToIndex(Position const& p) const {
	return this->m_width * p.w + p.h;
}

QImage Maze::toImage(int scaleX, int scaleY) const {
	QImage result(m_width * scaleX, m_height * scaleY, QImage::Format_ARGB32_Premultiplied);

	QPainter p;
	p.begin(&result);

	QColor const cWhite = Qt::white;
	QColor const cBlack = Qt::black;
	QColor const cYellow = Qt::yellow;

	p.fillRect(0, 0, m_width * scaleX, m_height * scaleY, cWhite);
	for (int w = 0; w < m_width; ++w) {
		for (int h = 0; h < m_height; ++h) {
			Position const currentPos(w, h);
			Cell const c = m_cells.at(coordToIndex(currentPos));
			if (c == Cell::Blocked) {
				p.fillRect(w * scaleX, h * scaleY, scaleX, scaleY, cBlack);
			} else if (c == Cell::Path) {
				p.fillRect(w * scaleX, h * scaleY, scaleX, scaleY, cYellow);
			} else {
				p.fillRect(w * scaleX, h * scaleY, scaleX, scaleY, cWhite);
			}
		}
	}

	p.end();
	return result;
}

void Maze::print() const {
	for (int h = 0; h < m_height; ++h) {
		for (int w = 0; w < m_width; ++w) {
			Position const currentPos(w, h);
			Cell const c = m_cells.at(coordToIndex(currentPos));
			if (c == Cell::Blocked) {
				std::cout << "X";
			} else if (c == Cell::Path) {
				std::cout << "-";
			} else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

bool Maze::isLegal(Position const& p) const {
	return p.w > 0 && p.w < m_width - 1 && p.h > 0 && p.h < m_height - 1;
}

std::set<Maze::Position> Maze::getFrontier(Position const& p) const {
	std::set<Maze::Position> result;
	addFrontier(result, p);
	return result;
}

void Maze::addFrontier(std::set<Position>& frontier, Position const& p) const {
	std::vector<Position> const corners = { {p.w - 2, p.h}, { p.w + 2, p.h }, { p.w, p.h - 2 }, { p.w, p.h + 2 } };
	for (auto it = corners.cbegin(); it != corners.cend(); ++it) {
	//for (int w = p.w - 2; w <= p.w + 2; w += 2) {
//		for (int h = p.h - 2; h <= p.h + 2; h += 2) {
			//if (w == h) continue;
			Position const c = *it;
			if (isLegal(c) && m_cells.at(coordToIndex(c)) == Cell::Blocked) {
				frontier.insert(c);
			}
		//}
	}
}

std::vector<Maze::Position> Maze::getNeighbor(Position const& p) const {
	std::vector<Maze::Position> result;

	std::vector<Position> const corners = { {p.w - 2, p.h}, { p.w + 2, p.h }, { p.w, p.h - 2 }, { p.w, p.h + 2 } };
	for (auto it = corners.cbegin(); it != corners.cend(); ++it) {
	//for (int w = p.w - 2; w <= p.w + 2; w += 2) {
		//for (int h = p.h - 2; h <= p.h + 2; h += 2) {
			//if (w == h) continue;
			Position const c = *it;
			if (isLegal(c) && m_cells.at(coordToIndex(c)) == Cell::Passage) {
				result.push_back(c);
			}
		//}
	}

	return result;
}

int betweenInner(int a, int b) {
	int const diff = b - a;
	if (diff == 0) {
		return a;
	} else if (diff == 2) {
		return 1 + a;
	} else if (diff == -2) {
		return -1 + a;
	} else {
		throw;
	}
}

Maze::Position Maze::between(Position const& p1, Position const& p2) const {
	return Position(betweenInner(p1.w, p2.w), betweenInner(p1.h, p2.h));
}

bool Maze::connectRandomNeighbor(Position const& p) {
	std::vector<Maze::Position> const neighbors = getNeighbor(p);
	if (neighbors.size() == 0) {
		return false;
	}
	int const pickedIndex = QRandomGenerator::global()->bounded(0, static_cast<int>(neighbors.size()));
	Position const n = neighbors.at(pickedIndex);
	Position const posBetween = between(p, n);
	m_cells.at(coordToIndex(posBetween)) = Cell::Passage;
	return true;
}

void Maze::extend(std::set<Position>& frontier) {
	while (frontier.size() > 0) {
		int const pickedIndex = QRandomGenerator::global()->bounded(0, static_cast<int>(frontier.size()));

		auto it = frontier.begin();
		std::advance(it, pickedIndex);

		Position const f = *it;
		m_cells.at(coordToIndex(f)) = Cell::Passage;
		frontier.erase(it);
		if (connectRandomNeighbor(f)) {
			addFrontier(frontier, f);
		}
	}
}

namespace std {
	template <>
	struct hash<Maze::Position> {
		size_t operator()(Maze::Position const& k) const {
			// Compute individual hash values for first and second
			// http://stackoverflow.com/a/1646913/126995
			size_t res = 17;
			res = res * 31 + hash<int>()(k.w);
			res = res * 31 + hash<int>()(k.h);
			return res;
		}
	};

	template <>
	struct equal_to<Maze::Position> {
		bool operator()(Maze::Position const& x, Maze::Position const& y) const {
			return x == y;
		}
	};
}

std::vector<Maze::Position> Maze::findPath(Position const& from, Position const& to) const {
	if (m_cells.at(coordToIndex(from)) == Cell::Blocked) {
		return {};
	} else if (m_cells.at(coordToIndex(to)) == Cell::Blocked) {
		return {};
	}

	std::set<Position> seen;
	std::queue<Position> q;
	std::unordered_map<Position, Position> path;
	q.push(from);
	seen.insert(from);
	while (!q.empty()) {
		Position const p = q.front();
		q.pop();

		std::vector<Position> const neighbors = { {p.w - 1, p.h}, { p.w + 1, p.h }, { p.w, p.h - 1 }, { p.w, p.h + 1 } };
		for (auto it = neighbors.cbegin(); it != neighbors.cend(); ++it) {
			Position const& neighbor = *it;
			if (!isLegal(neighbor) || m_cells.at(coordToIndex(neighbor)) == Cell::Blocked) continue;

			if (seen.find(neighbor) == seen.end()) {
				q.push(neighbor);
				seen.insert(neighbor);
				path.insert({ neighbor, p });

				if (neighbor.w == to.w && neighbor.h == to.h) {
					// Trace path
					std::vector<Position> result;
					Position cur = to;
					while (true) {
						result.push_back(cur);
						if (cur.w == from.w && cur.h == from.h) return result;
						cur = path.at(cur);
					}

					throw;
				}
			}
		}
	}

	return {};
}

void Maze::markPath() {
	for (auto const& p : m_path) {
		m_cells.at(coordToIndex(p)) = Cell::Path;
	}
}

double Maze::getPathAverageDistanceToCenter() const {
	double const centerW = m_width / 2.0;
	double const centerH = m_height / 2.0;
	double sum = 0;
	for (auto const& p : m_path) {
		int const diffW = centerW - p.w;
		int const diffH = centerH - p.h;
		sum += std::sqrt(diffW * diffW + diffH * diffH);
	}
	return sum / m_path.size();
}

int Maze::getWidth() const {
	return m_width;
}

int Maze::getHeight() const {
	return m_height;
}

std::vector<Maze::Position> const& Maze::getPath() const {
	return m_path;
}

std::size_t Maze::getPathLength() const {
	return m_path.size();
}

Maze::Cell const& Maze::getCell(int w, int h) const {
	return m_cells.at(coordToIndex({ w, h }));
}
