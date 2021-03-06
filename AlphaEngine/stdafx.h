#pragma once

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <set>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <windowsx.h>
#include <comdef.h>
#include <pix.h>

#define _XM_NO_INTRINSICS_
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

#include "DDSTextureLoader.h"
#include "MathHelper.h"
#include "d3dx12.h"
#include "d3dUtil.h"

#include "ADefine.h"
#include "Singleton.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace Microsoft::WRL;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

