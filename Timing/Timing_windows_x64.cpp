#include	"Timing_windows_x64.hpp"

using namespace std;

void ProfilerDp::start ( int aBlockId )
{
  mStart = ReadTSC();

}


void ProfilerDp::end ( int aBlockId )
{
  mEnd = ReadTSC();
  Measurement m;
  m.totalTime = time();
  mTimings[aBlockId].push_back ( m );
}

long long ProfilerDp::time()
{
  return mEnd-mStart;
}

void ProfilerDp::dumpToFile(LPCWSTR aFileName)
{
  std::ofstream     file;
  file.open ( aFileName );

  for (int i = 0; i < mNumTests; ++i)
  {
    file << "Block tested \"" << mBlockNames[i] <<"\":"<<endl;
    for (unsigned int j = 0; j < mTimings[i].size() ; ++j)
    {
      file <<  mTimings[i][j].totalTime <<endl;
    }
    file<<endl;
  }

  file.close();
}

ProfilerDp::ProfilerDp(int aNumBlocks, std::vector<string>& aBlockNames)
{
  mTimings.resize( aNumBlocks );
  mNumTests = aNumBlocks;
  mBlockNames = aBlockNames;
}

