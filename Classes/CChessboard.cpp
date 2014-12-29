/*
 * CChessboard.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: tangxinming
 */

#include "CChessboard.h"
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;



static void buildTrigon(int idx, int N, int M, TVecPos& out)
{
    const static SPos o[6] =
    {
        SPos((N-1)*3-(M-1),     (N-1)*4-(M-1)),
        SPos((N-1)*6-(M-1)*2,   (N-1)*3),
        SPos((N-1)*6-(M-1)*2,   (N-1)),
        SPos((N-1)*3-(M-1),      M-1),
        SPos(0,                  N-1),
        SPos(0,                 (N-1)*3)
    };
    
    const static int K[6] =
    {
        1,
        -1,
        1,
        -1,
        1,
        -1
    };
    
    const SPos & from = o[idx];
    int k = K[idx];
    
    for (int y=0; y<=(M-1); y++)
        for (int x=0+y; x <=(M-1)*2-y; x++)
        {
            if ((x+y) % 2 == (N+1) % 2)
            	out.push_back(SPos(from.x + x, from.y + k*y));
        }
}


CChessboard::CChessboard(int N_boardWidth, int M_chessWidth) {
	// TODO Auto-generated constructor stub
    m_boardWidth = N_boardWidth;
    m_chessCount = M_chessWidth;
    InitChesses();
}

CChessboard::~CChessboard() {
	// TODO Auto-generated destructor stub
}

void CChessboard::InitChesses()
{
    const int N = m_boardWidth;
    
    // 获得整个棋盘
    for (int x=0; x<=(N-1)*2*3; x++)
        for (int y=0; y <=(N-1)*4; y++) {
            SPos tmpPos(x,y);
            if (IsInBoard(N, SPos(x,y)))
                m_board[tmpPos] = e_none;
        }
    
    const int M = m_chessCount;
    // red
    TVecPos initPoses;
    buildTrigon(4, N, M, initPoses);
    buildTrigon(1, N, M, m_winPoses[e_red]);
    for (TVecPos::iterator it = initPoses.begin(); it != initPoses.end(); ++it)
        m_board[*it] = e_red;
    
    
    // yellow
    initPoses.clear();
    buildTrigon(2, N, M, initPoses);
    buildTrigon(5, N, M, m_winPoses[e_yellow]);
    for (TVecPos::iterator it = initPoses.begin(); it != initPoses.end(); ++it)
        m_board[*it] = e_yellow;
    
    // blue
    initPoses.clear();
    buildTrigon(0, N, M, initPoses);
    buildTrigon(3, N, M, m_winPoses[e_blue]);
    for (TVecPos::iterator it = initPoses.begin(); it != initPoses.end(); ++it)
        m_board[*it] = e_blue;
}

bool CChessboard::IsOccupied(const TMapBoard& board, const SPos& pos)
{
    TMapBoard::const_iterator it = board.find(pos);
    if (it != board.end()) {
        return it->second != e_none;
    }
    return false;
}


// 得到当前棋盘
void CChessboard::GetBoard(CChessboard::TVecChessPoint & vecChessPoint) const
{
    for (TMapBoard::const_iterator it = m_board.begin(); it != m_board.end(); ++it) {
        CChessboard::TVecChessPoint::value_type v;
        v.pos = it->first;
        v.type = it->second;
        vecChessPoint.push_back(v);
    }
}

bool CChessboard::GetMovables(const SPos& pos, CChessboard::TMapPath &foundPos) const
{
    TMapBoard::const_iterator it = m_board.find(pos);
    if (it == m_board.end())
        return false;
    if (it ->second == e_none)
        return false;
    
    TVecPos vecNext;
    vecNext.push_back(pos);
    TMapBoard finded;
    
    TVecPos neighbours;
    GetNeighbours(m_boardWidth, pos, neighbours);
    for (TVecPos::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
        if (!IsOccupied(m_board, *it))  // 只走一格的情况
        {
            foundPos[*it] = pos;
        }
    }

    SearchPoses(m_board, m_boardWidth, vecNext, finded, foundPos);
    return true;
}

void CChessboard::SearchPoses(const CChessboard::TMapBoard &theBorad,
                              int N,
                              TVecPos &vecJump,
                              CChessboard::TMapBoard &findedPos,
                              CChessboard::TMapPath &foundPos)
{
    if (vecJump.size() == 0)
        return;
    
    if (vecJump.size() != 0)
    {
        SPos curPos = *vecJump.rbegin();
        vecJump.pop_back();
        findedPos[curPos] = e_none;
        
        TVecPos neighbours;
        GetNeighbours(N, curPos, neighbours);
        for (TVecPos::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
            if (IsOccupied(theBorad, *it))  // 这一格被占领了，寻找次方向上点下一点，放入vecNextPos
            {
                TVecPos ne2;
                GetJumped(N, curPos, *it, ne2);
                for (TVecPos::iterator it2=ne2.begin(); it2 != ne2.end(); ++it2) {
                    if (!IsOccupied(theBorad, *it2)) {
                        if (findedPos.find(*it2) == findedPos.end()) {
                            findedPos[*it2] = e_none;
                            vecJump.push_back(*it2);
                            foundPos[*it2] = curPos;
                        }
                    }
                }
            }
        }
    }
    SearchPoses(theBorad, N, vecJump, findedPos, foundPos);
}


CChessboard::EChessType CChessboard::getWinner()
{
    for (int i=0; i<sizeof(m_winPoses)/sizeof(m_winPoses[0]); i++) {
        const TVecPos & vecPos = m_winPoses[i];
        EChessType retval = (EChessType)i;
        for (TVecPos::const_iterator it = vecPos.begin(); it != vecPos.end(); ++it) {
            if (m_board[*it] != retval)
            {
                retval = e_none;
                break;
            }
        }
        if (retval != e_none) {
            return retval;
        }
    }
    return e_none;
}

bool CChessboard::DoMove(const SPos& fromPos, const SPos& toPos, TVecPos& outPath)
{
    TMapBoard::const_iterator it = m_board.find(fromPos);
    if (it == m_board.end() || it->second == e_none)
        return false;
    EChessType eType = it->second;
    
    TMapPath mapPath;
    GetMovables(fromPos, mapPath);
    TMapPath::iterator itPath = mapPath.find(toPos);
    if (itPath == mapPath.end()) {
        return false;
    }
    
    for (SPos curPoint = toPos; curPoint != fromPos; ) {
        outPath.push_back(curPoint);
        curPoint = mapPath[curPoint];
    }
    
    m_board[fromPos] = e_none;
    m_board[toPos] = eType;
    return true;
}


bool CChessboard::IsInBoard(int N, const SPos& pos)
{
    if (pos.y < (N-1)) {
        if (pos.x + pos.y<(N-1)*3)
            return false;
        else if (pos.x -pos.y >(N-1)*3)
            return false;
        else
            goto SUCCESS;
    }
    else if (pos.y == (N-1))
    {
        if (pos.x >= 0 && pos.x <=(N-1)*2*3)
            goto SUCCESS;
        else
            return false;
    }
    else if (pos.y == (N-1)*3)
    {
        if (pos.x >= 0 && pos.x <=(N-1)*2*3)
            goto SUCCESS;
        else
            return false;
    }
    else if (pos.y > (N-1)*3) {
        if (pos.x - pos.y < -(N-1))
            return false;
        else if (pos.x + pos.y > 7*(N-1))
            return false;
        else
            goto SUCCESS;
    }
    else
    {
        if ((pos.x + pos.y < (N-1)*3) && (pos.x - pos.y < -(N-1)))
            return false;
        else if ((pos.x - pos.y>(N-1)*3) && (pos.x + pos.y>7*(N-1)))
            return false;
        else
            goto SUCCESS;
    }
    
SUCCESS:
    if ((pos.x + pos.y) % 2 == (N+1) % 2)
        return true;
    else
        return false;
}

void CChessboard::test()
{
//    CChessboard cb;
//    auto counter = cb.m_board.size();
//    assert(counter == 73);
//    
//    int N = 4;
//    TVecPos pos1;
//    GetNeighbours(N, SPos(9, 0), pos1);
//    assert(pos1.size() == 2);
//    
//    TVecPos pos2;
//    GetNeighbours(N, SPos(0, 3), pos2);
//    assert(pos2.size() == 2);
//    
//    TVecPos pos3;
//    GetNeighbours(N, SPos(12, 9), pos3);
//    assert(pos3.size() == 5);
//    
//    TVecPos pos4;
//    GetNeighbours(N, SPos(5, 4), pos4);
//    assert(pos4.size() == 6);
//    
//    TMapPath out;
//    cb.GetMovables(SPos(9, 0), out);
//    assert(out.size() == 0);
//    
//    out.clear();
//    cb.GetMovables(SPos(12, 3), out);
//    assert(out.size() == 3);
//
//    out.clear();
//    cb.GetMovables(SPos(9, 2), out);
//    assert(out.size() == 2);
//    
//    TVecPos path;
//    cb.DoMove(SPos(11, 2), SPos(9, 4), path);
//    assert(path.size() == 1);
//    
//    out.clear();
//    cb.GetMovables(SPos(9, 0), out);
//    assert(out.size() == 2);
//    
//    path.clear();
//    cb.DoMove(SPos(9, 0), SPos(13, 4), path);
//    assert(path.size() == 2);
//    
    CChessboard cb2(5, 4);
    assert(cb2.m_board.size() == 121);
}

void CChessboard::GetNeighbours(int N, const SPos& pos, TVecPos& neighbours)
{
    if (IsInBoard(N, pos)) {
        for (int x=pos.x-2; x<=pos.x+2; x++) {
            for (int y=pos.y-1; y <=pos.y+1; y++) {
                SPos tempPos(x,y);
                if (tempPos != pos && IsInBoard(N, tempPos)) {
                    neighbours.push_back(tempPos);
                }
            }
        }
    }
}

void CChessboard::GetJumped(int N, const SPos& source, const SPos& between, TVecPos& out)
{
    int dx = between.x - source.x;
    int dy = between.y - source.y;
    SPos dst((between.x + dx), (between.y+dy));
    if (IsInBoard(N, dst)) {
        out.push_back(dst);
    }
}
