#include "UI_Textures/SignalLayerWidget.hpp"
#include "Timing/Timing_windows_x64.hpp"

void  SignalLayerWidget::profileShaderCompile()
{

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_1_base");
    LPCWSTR proftext (L"prof_1_base.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_2_includes");
    LPCWSTR proftext (L"prof_2_includes.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }


  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_3_loop");
    LPCWSTR proftext (L"prof_3_loops.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_4_simplex");
    LPCWSTR proftext (L"prof_4_simplex.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }
/*
  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_5_complete");
    LPCWSTR proftext (L"prof_5_complete.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_6_less_code");
    LPCWSTR proftext (L"prof_6_less_code.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_7_no_bump");
    LPCWSTR proftext (L"prof_7_no_bump.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_8_minimize_variables");
    LPCWSTR proftext (L"prof_8_minimize_variables.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_9_bump_not_colliding");
    LPCWSTR proftext (L"prof_9_bump_not_colliding.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }


{
  std::vector<std::string> blocks (1);
  blocks[0] = std::string("Compile shader via DX");
  ProfilerDp profiler( 1, blocks );  //just pointer

  std::string shader ("prof_10_simplified_bump");
  LPCWSTR proftext (L"prof_10_simplified_bump.txt");
  for (int it=0;it<10;++it)
    profileRedrawFinal( shader, &profiler);
  profiler.dumpToFile( proftext );
}

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_11_bump_test");
    LPCWSTR proftext (L"prof_11_bump_test.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_12_bump_min");
    LPCWSTR proftext (L"prof_12_bump_min.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_13_one_inc");
    LPCWSTR proftext (L"prof_13_one_inc.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }


  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_14_bump_min");
    LPCWSTR proftext (L"prof_14_bump_min.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }
*/

  {
    std::vector<std::string> blocks (1);
    blocks[0] = std::string("Compile shader via DX");
    ProfilerDp profiler( 1, blocks );  //just pointer

    std::string shader ("prof_15_bump_min");
    LPCWSTR proftext (L"prof_15_bump_min.txt");
    for (int it=0;it<10;++it)
      profileRedrawFinal( shader, &profiler);
    profiler.dumpToFile( proftext );
  }

  exit(1);
}
