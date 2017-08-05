// File: MikuMikuDance.cpp
// Author: Rendong Liang (Liong)
#include "MikuMikuDance.hpp"

/*
 * Bone deformation:
 *                     BDEF1 BDEF2 BDEF4   SDEF  QDEF
 *   [float] Bone   #1   X     X     X      X     X
 *   [float] Bone   #2         X     X      X     X
 *   [float] Bone   #3               X            X
 *   [float] Bone   #4               X            X
 *   [float] Weight #1         X     X      X     X
 *   [float] Weight #2               X            X
 *   [float] Weight #3               X            X
 *   [float] Weight #4               X            X
 *   [vec3]  C                              X
 *   [vec3]  R0                             X
 *   [vec3]  R1                             X
 *   Byte Num              4    12    32     48    32
 *   So 4 [vec3] is used to carry vertex data to shaders.
 */


