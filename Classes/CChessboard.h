/*
 * CChessboard.h
 *
 *  Created on: Nov 27, 2014
 *      Author: tangxinming
 */

#ifndef CCHESSBOARD_H_
#define CCHESSBOARD_H_

#include <vector>
#include <map>

typedef struct _SPos {
    _SPos()
    {
        this->x = 0;
        this->y = 0;
    }
    
    _SPos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
	int x;
	int y;
    bool operator != (const _SPos& pos) const
    {
        bool retval = (pos == *this);
        return !retval;
    }
    
    bool operator == (const _SPos& pos) const
    {
        return pos.x == x && pos.y == y;
    }
	bool operator < (const _SPos& pos) const
	{
		if (x < pos.x)
			return true;
		else if (x == pos.x)
			return y < pos.y;
		else
			return false;
	}
}SPos;

typedef std::vector<SPos> TVecPos;

class CChessboard {
public:
	CChessboard(int N_boardWidth=4, int M_chessWidth=4);
	virtual ~CChessboard();

	typedef enum _EChessType {
		e_none=-1,
		e_red,
		e_blue,
		e_yellow
	}EChessType;

    typedef struct _SChessPoint
    {
        SPos pos;
        EChessType type;
    }SChessPoint;
    typedef std::vector<SChessPoint> TVecChessPoint;
    typedef std::map<SPos, EChessType> TMapBoard;
    typedef std::map<SPos, SPos> TMapPath;
    
public:
    // 得到当前棋盘
    void GetBoard(CChessboard::TVecChessPoint & vecChessPoint) const;
    
	bool GetMovables(const SPos& pos, CChessboard::TMapPath &foundPos) const;
    bool DoMove(const SPos& fromPos, const SPos& toPos, TVecPos& outPath);
	EChessType getWinner();

    static void test();
protected:
	TMapBoard m_board;      // 棋盘
	TVecPos m_winPoses[3];	// 3种颜色的棋子胜利时所处的位置
	void InitChesses();
    static bool IsOccupied(const TMapBoard& board, const SPos& pos);

	static bool IsInBoard(int N, const SPos& pos);
    static void GetNeighbours(int N, const SPos& pos, TVecPos& neighbours);
    static void GetJumped(int N, const SPos& source, const SPos& between, TVecPos& out);
    static void SearchPoses(const CChessboard::TMapBoard &theBorad,
                            int N,
                            TVecPos &vecJump,
                            CChessboard::TMapBoard &findedPos,
                            CChessboard::TMapPath &foundPos);
    
    
protected:
    int m_boardWidth;
    int m_chessCount;
};

#endif /* CCHESSBOARD_H_ */
