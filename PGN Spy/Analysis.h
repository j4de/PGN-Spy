// MIT License
// 
// Copyright(c) 2016 Michael J. Gleason
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

class CAnalysisSettings
{
public:
   CAnalysisSettings();
   bool LoadSettings();
   bool SaveSettings();

   CString m_sPlayerName;
   BOOL m_bExcludeForcedMoves;
   int m_iForcedMoveCutoff; //in centipawns, will exclude moves where the second move is this much worse than the best
   BOOL m_bIncludeOnlyUnclearPositions;
   int m_iUnclearPositionCutoff; //in centipawns, will exclude moves where the Tn position is this much worse than the best
   int m_iBlunderThreshold; //in centipawns, moves that lose more than this are considered blunders
   int m_iEqualPositionThreshold; //in centipawns, restricts analysis to positions where neither side is winning by more than this value
   int m_iLosingThreshold; //in centipawns, restricts analysis to positions where the side in question is losing by more than the equal position threshold, but by less than this value
   int m_iBookDepth;
   CString m_sEnginePath;
   int m_iNumVariations;
   int m_iSearchDepth;
   int m_iMaxTime;
   int m_iMinTime;
   int m_iNumThreads;
   int m_iHashSize; //in MB
};

class CMove
{
public:
   CMove();
   CString m_sMove;
   int m_iDepth;
   int m_iTime;
   int m_iScore;
};

class CPosition
{
public:
   CPosition();
   CPosition(const CPosition &rSrc);
   CPosition &operator=(const CPosition &rSrc);
   bool IsForcedMove(int iVariation, int iForcedMoveThreshold);
   bool IsUnclearPosition(int iVariation, int iUnclearPositionThreshold);
   bool IsBlunder(int iBlunderThreshold);
   bool IsEqualPosition(int iEqualPositionThreshold);
   bool IsLosingPosition(int iEqualPositionThreshold, int iLosingPositionThreshold);
   int GetCentipawnLoss();
   CArray<CMove, CMove> m_avTopMoves;
   int m_iMovePlayed;
};

class CGame
{
public:
   CGame() {}
   CGame(const CGame &rSrc);
   CGame &operator=(const CGame &rSrc);
   bool LoadGame(CString sGameText);

   CArray<CPosition, CPosition> m_avPositions;
   CString m_sEvent;
   CString m_sDate;
   CString m_sWhite;
   CString m_sBlack;
   CString m_sResult;
   CString m_sTimeControl;
};

class CStats
{
public:
   CStats();
   int m_iNumPositions;
   CArray<int, int> m_aiTValues; //T1/T2/T3/etc. totals
   CArray<int, int> m_aiTMoves; //T1/T2/T3/etc. totals
   int m_iBlunders;
   int m_iTotalCentipawnLoss;
   CArray<int, int> m_aiCentipawnLosses; //for calculating std deviation

   double m_dAvgCentipawnLoss;
   double m_dCentipawnLossStdDeviation;

   void Initialize(const CAnalysisSettings &vSettings);
   void AddPosition(CPosition &vPosition, const CAnalysisSettings &vSettings);
   void FinaliseStats();
};

class CGamePGN
{
public:
   CGamePGN() {}
   CString m_sPGNText;
   CString m_sWhite;
   CString m_sBlack;
   CString m_sFileName;
};

class CAnalyser
{
public:
   CAnalyser();
   CAnalysisSettings m_vAnalysisSettings;
   int m_iGamesWithErrors;
   CArray<HANDLE, HANDLE> m_ahChildStdInRead;
   CArray<HANDLE, HANDLE> m_ahChildStdInWrite;
   CArray<HANDLE, HANDLE> m_ahChildStdOutRead;
   CArray<HANDLE, HANDLE> m_ahChildStdOutWrite;
   CArray<HANDLE, HANDLE> m_ahChildStdErrRead;
   CArray<HANDLE, HANDLE> m_ahChildStdErrWrite;
   CArray<HANDLE, HANDLE> m_ahProcesses;
   CArray<CGame, CGame> m_avGames;
   CString m_sMessage;
   bool ProcessGames(CString sConvertedPGN);
   bool LaunchAnalyser(CGamePGN vGamePGN, int iCurThread);
   void ReadFromThread(int iThread, CString IN OUT &rsResults, bool IN OUT &rbError);
   void ProcessOutput(CString sOutput, BOOL bExcludeForcedMoves);
};