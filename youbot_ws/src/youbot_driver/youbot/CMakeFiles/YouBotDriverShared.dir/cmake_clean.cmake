FILE(REMOVE_RECURSE
  "../lib/libYouBotDriver.pdb"
  "../lib/libYouBotDriver.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/YouBotDriverShared.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
