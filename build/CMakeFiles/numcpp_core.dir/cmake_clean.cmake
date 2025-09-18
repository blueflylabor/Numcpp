file(REMOVE_RECURSE
  "libnumcpp_core.a"
  "libnumcpp_core.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/numcpp_core.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
