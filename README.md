# res-cpp
custom resource managment system (similar to this in java)

## usage:
```
res <output> <resources>
```
## res-cpp logic
res-cpp reads every program into memory and creates header file.
header file contains every file in resources directory.
illegal characters are replaced with '_'

Program is hobby project and not done for production use case!

## build:
```
g++ main.cpp --static -O3 -o bf
```
