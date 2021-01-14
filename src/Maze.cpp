#include "Maze.h"

#include <QBrush>
#include <QColor>
#include <QPainter>

#include <iostream>
#include <random>
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
		Position const start(getRandomNumber(0, m_width - 1), getRandomNumber(0, m_height - 1));
		m_cells.at(coordToIndex(start)) = Cell::Passage;
		std::set<Maze::Position> frontier = getFrontier(start);
		extend(frontier);

		int const indexLeft = getIndexOfFirstNonBlackRow(Direction::LEFT);
		int const indexRight = getIndexOfFirstNonBlackRow(Direction::RIGHT);
		int const indexTop = getIndexOfFirstNonBlackRow(Direction::TOP);
		int const indexBottom = getIndexOfFirstNonBlackRow(Direction::BOTTOM);

		// Enable entry and exit
		for (int y = 0; y < indexTop; ++y) {
			m_cells.at(coordToIndex(indexLeft, y)) = Cell::Passage;
		}
		for (int y = m_height - 1; y > indexBottom; --y) {
			m_cells.at(coordToIndex(indexRight, y)) = Cell::Passage;
		}
		m_path = findPath({ indexLeft, indexTop }, { indexRight, indexBottom });
		//toImage(10, 10).save(QString("maze_%1.png").arg(redoCounter));
	} while (ensurePathFromTopLeftToBottomRightExists && m_path.empty() || ((minLength > 0) && (m_path.size() < minLength)));

	if (!m_path.empty() && markPath) {
		this->markPath();
	}
}

int Maze::getIndexOfFirstNonBlackRow(Direction dir) const {
	if (dir == Direction::LEFT) {
		for (int w = 0; w < m_width; ++w) {
			for (int h = 0; h < m_height; ++h) {
				if (m_cells.at(coordToIndex(w, h)) != Cell::Blocked) {
					return w;
				}
			}
		}
	} else if (dir == Direction::RIGHT) {
		for (int w = m_width - 1; w >= 0; --w) {
			for (int h = 0; h < m_height; ++h) {
				if (m_cells.at(coordToIndex(w, h)) != Cell::Blocked) {
					return w;
				}
			}
		}
	} else if (dir == Direction::TOP) {
		for (int h = 0; h < m_height; ++h) {
			for (int w = 0; w < m_width; ++w) {
				if (m_cells.at(coordToIndex(w, h)) != Cell::Blocked) {
					return h;
				}
			}
		}
	} else if (dir == Direction::BOTTOM) {
		for (int h = m_height - 1; h >= 0; --h) {
			for (int w = 0; w < m_width; ++w) {
				if (m_cells.at(coordToIndex(w, h)) != Cell::Blocked) {
					return h;
				}
			}
		}
	}
	throw;
}

int Maze::getRandomNumber(int min, int max) {
	static std::random_device rd;
	static std::ranlux48 gen(rd());
	std::uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}

int Maze::coordToIndex(Position const& p) const {
	return this->m_width * p.w + p.h;
}

int Maze::coordToIndex(int w, int h) const {
	return this->m_width * w + h;
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
			Cell const c = m_cells.at(coordToIndex(w, h));
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
			Cell const c = m_cells.at(coordToIndex(w, h));
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

bool Maze::isLegal(int w, int h) const {
	return w > 0 && w < m_width - 1 && h > 0 && h < m_height - 1;
}

std::set<Maze::Position> Maze::getFrontier(Position const& p) const {
	std::set<Maze::Position> result;
	addFrontier(result, p);
	return result;
}

void Maze::addFrontierSub(std::set<Position>& frontier, int w, int h) const {
	if (isLegal(w, h) && m_cells.at(coordToIndex(w, h)) == Cell::Blocked) {
		frontier.insert({ w, h });
	}
}

void Maze::addFrontier(std::set<Position>& frontier, Position const& p) const {
	addFrontierSub(frontier, p.w - 2, p.h);
	addFrontierSub(frontier, p.w + 2, p.h);
	addFrontierSub(frontier, p.w, p.h - 2);
	addFrontierSub(frontier, p.w, p.h + 2);
}

void Maze::getNeighborsSub(std::vector<Maze::Position>& result, int w, int h) const {
	if (isLegal(w, h) && m_cells.at(coordToIndex(w, h)) == Cell::Passage) {
		result.push_back({w, h});
	}
}

void Maze::getNeighbors(std::vector<Position>& neighbors, Position const& p) const {
	neighbors.clear();

	getNeighborsSub(neighbors, p.w - 2, p.h);
	getNeighborsSub(neighbors, p.w + 2, p.h);
	getNeighborsSub(neighbors, p.w, p.h - 2);
	getNeighborsSub(neighbors, p.w, p.h + 2);
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
	static std::vector<Maze::Position> neighbors;
	getNeighbors(neighbors, p);
	if (neighbors.size() == 0) {
		return false;
	}
	int const pickedIndex = getRandomNumber(0, static_cast<int>(neighbors.size()) - 1);
	Position const n = neighbors.at(pickedIndex);
	Position const posBetween = between(p, n);
	m_cells.at(coordToIndex(posBetween)) = Cell::Passage;
	return true;
}

void Maze::extend(std::set<Position>& frontier) {
	while (frontier.size() > 0) {
		int const pickedIndex = getRandomNumber(0, static_cast<int>(frontier.size()) - 1);

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
