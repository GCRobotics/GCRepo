FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/GCRobotics/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/GCRobotics/msg/__init__.py"
  "../src/GCRobotics/msg/_i2cData.py"
  "../src/GCRobotics/msg/_simpleVelocity.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
