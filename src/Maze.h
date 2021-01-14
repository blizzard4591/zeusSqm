#ifndef ZEUSSQM_MAZE_H_
#define ZEUSSQM_MAZE_H_

#include <functional>
#include <set>
#include <utility>
#include <vector>

#include <QImage>

// Based on https://stackoverflow.com/a/29758926

class Maze {
public:
	Maze(int width, int height, bool ensurePathFromTopLeftToBottomRightExists, bool markPath, int minLength);
	virtual ~Maze() {}

	enum class Cell {
		Blocked, Passage, Path
	};

	//typedef std::pair<int, int> Position;
	struct Position {
		Position(int w, int h) : w(w), h(h) {}

		bool operator==(Position const& other) const {
			return (w == other.w && h == other.h);
		}

		bool operator<(Position const& other) const {
			return (w < other.w || (w == other.w && h < other.h));
		}

		int w;
		int h;
	};

	QImage toImage(int scaleX, int scaleY) const;
	void print() const;
	
	int getWidth() const;
	int getHeight() const;
	std::vector<Position> const& getPath() const;
	std::size_t getPathLength() const;
	double getPathAverageDistanceToCenter() const;
	Cell const& getCell(int w, int h) const;
private:
	int const m_width;
	int const m_height;
	std::vector<Cell> m_cells;
	std::vector<Position> m_path;

	int coordToIndex(Position const& p) const;

	bool isLegal(Position const& p) const;
	std::set<Position> getFrontier(Position const& p) const;
	void addFrontier(std::set<Position>& frontier, Position const& p) const;
	std::vector<Position> getNeighbor(Position const& p) const;
	bool connectRandomNeighbor(Position const& p);
	Position between(Position const& p1, Position const& p2) const;
	void extend(std::set<Position>& frontier);

	std::vector<Position> findPath(Position const& from, Position const& to) const;
	void markPath();
};

#endif
