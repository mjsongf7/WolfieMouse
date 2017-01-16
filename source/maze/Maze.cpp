#include <common_maze.h>
#include <Maze.hpp>
#include <stdio.h>

void Maze::init()
{
    int i = 0;
    int j = 0;
    int k = 0;

    index_start_row = 0;
    index_start_col = 0;
    index_goal_row = 7;
    index_goal_col = 7;

    /* init the wall with unknown */
    for (i = 0; i < CONFIG_MAX_ROW_SIZE; i++) {
        for (j = 0; j < CONFIG_MAX_COL_SIZE; j++) {
            setDistance(i, j, MAZE_UNREACHED);
            for (k = (int) row_plus; k <= (int) col_minus; k++) {
                setWall(i, j, (Direction) k, unknown);
            }
        }
    }

    /* wrap the wall */
    for (i = 0; i < CONFIG_MAX_ROW_SIZE; i++) {
        setWall(i, 0, col_minus, wall);
        setWall(i, CONFIG_MAX_ROW_SIZE - 1, col_plus, wall);
    }
    for (j = 0; j < CONFIG_MAX_COL_SIZE; j++) {
        setWall(0, j, row_minus, wall);
        setWall(CONFIG_MAX_COL_SIZE - 1, j, row_plus, wall);
    }

    /* fill the first cell */
    setWall(0, 0, row_plus, wall);
    setWall(0, 0, col_plus, empty);
    setWall(0, 0, row_minus, wall);
    setWall(0, 0, col_minus, wall);

    /* update the cell */
    updateCell();
}

Maze::Maze()
{
    init();
}

Maze::Maze(char *filename)
{
    init();
    readMazeFromFile(filename);
}

enum Wall Maze::getWall(int row, int col, Direction dir)
{
    if (MAZE_IS_POS_OUT_BOUNDS(row, col)) {
        return (enum Wall) COMMON_MAZE_ERROR;
    }
    switch (dir) {
    case row_plus:
        return rowWall[row + 1][col];
    case col_plus:
        return colWall[row][col + 1];
    case row_minus:
        return rowWall[row][col];
    case col_minus:
        return colWall[row][col];
    default:
        return (enum Wall) COMMON_MAZE_ERROR;
    }
    return (enum Wall) COMMON_MAZE_ERROR;
}

struct cell Maze::getCell(int row, int col)
{
    if (MAZE_IS_POS_OUT_BOUNDS(row, col)) {
        return (struct cell ) { -2, cellError, false, false, false } ;
    } else {
        return cell[row][col];
    }
}

int Maze::setWall(int row, int col, Direction dir, enum Wall status)
{
    if (MAZE_IS_POS_OUT_BOUNDS(row, col)) {
        return COMMON_MAZE_ERROR;
    }
    switch (dir) {
    case row_plus:
        rowWall[row + 1][col] = status;
        return COMMON_MAZE_SUCCESS;
    case col_plus:
        colWall[row][col + 1] = status;
        return COMMON_MAZE_SUCCESS;
    case row_minus:
        rowWall[row][col] = status;
        return COMMON_MAZE_SUCCESS;
    case col_minus:
        colWall[row][col] = status;
        return COMMON_MAZE_SUCCESS;
    default:
        return COMMON_MAZE_ERROR;
    }
}

int Maze::updateCell(int row, int col)
{
    if (MAZE_IS_POS_OUT_BOUNDS(row, col)) {
        return COMMON_MAZE_ERROR;
    }
    /* checking status */
    if (Maze::getWall(row, col, row_plus) != unknown
            && Maze::getWall(row, col, col_plus) != unknown
            && Maze::getWall(row, col, row_minus) != unknown
            && Maze::getWall(row, col, col_minus) != unknown) {
        cell[row][col].status = searched;
    } else {
        cell[row][col].status = unsearched;
    }
    /* checking goal */
    if ((row == index_goal_row) && (col == index_goal_col)) {
        cell[row][col].isGoal = true;
    } else {
        cell[row][col].isGoal = false;
    }
    /* checking start */
    if ((row == index_start_row) && (col == index_start_col)) {
        cell[row][col].isStart = true;
    } else {
        cell[row][col].isStart = false;
    }
    /* TODO: checking mouse */
    cell[row][col].isMouse = false;
    return COMMON_MAZE_SUCCESS;
}

void Maze::updateCell()
{
    int i = 0;
    int j = 0;
    for (i = 0; i < CONFIG_MAX_ROW_SIZE; i++) {
        for (j = 0; j < CONFIG_MAX_COL_SIZE; j++) {
            Maze::updateCell(i, j);
        }
    }
}

void Maze::setMouse(int row, int col)
{
    cell[row][col].isMouse = true;
}

void Maze::resetMouse(int row, int col)
{
    cell[row][col].isMouse = false;
}
