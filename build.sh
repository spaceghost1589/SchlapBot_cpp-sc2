#!/bin/bash
set -e
cmake --preset x64-RelWithDebInfo
cmake --build out/build/x64-RelWithDebInfo --parallel