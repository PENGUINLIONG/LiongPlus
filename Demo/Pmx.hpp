// File: Pmx.hpp
// Author: Rendong Liang (Liong)
#include <memory>
#pragma once
#include "LiongPlus/Array.hpp"
#include "LiongPlus/Buffer.hpp"
#include "LiongPlus/Matrix4x4.hpp"
#include "LiongPlus/SafeError.hpp"

using namespace LiongPlus;

/*
 * Polygon Model eXtended
 *
 * Pmx file is specialized model file format for MikuMikuDance.
 * The analized structure can be found here:
 *   https://gist.github.com/felixjones/f8a06bd48f9da9a4539f
 * The vertex data is ready to be given to shader after reading. So
 *   it somehow made the resolution procedure a bit chaotic.
 * The read data is not serialized for performance. It should be
 *   directly sent to shaders. The receiving shaders should have
 *   attributes (inputs) in the following format:
 *     [vec4] 
 */
struct Pmx
{
public:
	enum PmxResolutionErrorCode
	{
		Success_Perfect            = 0'000'0000,

		// The file signature or size cannot be acknowledged as a PMX
		//   file.
		Error_NotAcknowkedged      = 1'000'0001,
		// There are too few global properties.
		Error_UnsupportedGlobals   = 1'000'0010,
		// Error occured in text decoding process.
		Error_TextDecoding         = 1'000'0011,
		// File terminated at an unexpected position.
		Error_UnexpectedEof        = 1'000'0100,
		// Deform type code is not recognized.
		Error_UnsupportedDeform    = 1'000'0101,
		// Index size is invalid.
		Error_UnsupportedIndexSize = 1'000'0110,
		// Morph type code is not recognized.
		Error_UnsupportedMorph     = 1'000'0111,

		Error_Mask                 = 1'000'0000,
	};
	using PmxResolutionError = LiongPlus::SafeError<PmxResolutionErrorCode, uint8_t>;

	using pmx_index_t = int32_t;
	using pmx_size_t = int32_t;

	struct Vertex
	{
		enum class DeformType
		{
			BDef1 = 0, BDef2 = 1,
			BDef4 = 2, SDef  = 3,
			QDef  = 4,
		};
		struct DeformData
		{
			pmx_index_t Bone1Index = 0,
				Bone2Index = 0,
				Bone3Index = 0,
				Bone4Index = 0;
			float Bone1Weight = 0.f,
				Bone2Weight = 0.f,
				Bone3Weight = 0.f,
				Bone4Weight = 0.f;
			Vector4 C, R0, R1;
		};

		Vector4 Position;
		Vector4 Normal;
		Vector4 UV;
		Array<Vector4> AttachedVectors;
		DeformType DeformType;
		DeformData DeformData;
		// This value controls the drawing of model outline.
		// TODO: This should be implemented after all necessary works done.
		float EdgeScale;
	};
	struct Material
	{
		enum Flag
		{
			NoCulling     = 0b0000'0001,
			GroundShadow  = 0b0000'0010,
			DrawShadow    = 0b0000'0100,
			ReceiveShadow = 0b0000'1000,
			EdgePresent   = 0b0001'0000,
			VertexColor   = 0b0010'0000,
			DrawPoints    = 0b0100'0000,
			DrawWireFrame = 0b1000'0000,
		};
		enum EnvironmentBlendMode
		{
			Disabled = 0,
			Multiply = 1,
			Additive = 2,
			AttachedVector = 3,
		};
		enum ToonReference
		{
			TextureReference = 0,
			InternalReference = 1,
		};

		std::u32string NameLocal, NameUniversal, Metadata;
		Vector4 Diffuse;
		Vector4 Specular;
		float SpecularStrength;
		Vector4 Ambient;
		Flag Flags;
		Vector4 Edge;
		float EdgeScale;
		pmx_index_t TexIndex;
		pmx_index_t EnvIndex;
		EnvironmentBlendMode EnvBlendMode;
		ToonReference ToonRef;
		pmx_index_t ToonValue;
		pmx_size_t VertexCount;
	};
	struct Bone
	{
		enum Flag
		{
			IndexedTail          = 0b0000'0000'0000'0001,
			CanRotate            = 0b0000'0000'0000'0010,
			CanTranslate         = 0b0000'0000'0000'0100,
			IsVisible            = 0b0000'0000'0000'1000,
			IsEnabled            = 0b0000'0000'0001'0000,
			IsIKEnabled          = 0b0000'0000'0010'0000,
			// TODO:
			UNKNOWN_A            = 0b0000'0000'0100'0000,

			InheritLocalDeform   = 0b0000'0000'1000'0000,
			InheritRotation      = 0b0000'0001'0000'0000,
			InheritTranslation   = 0b0000'0010'0000'0000,
			HasFixedAxis         = 0b0000'0100'0000'0000,
			HasLocalCoordinate   = 0b0000'1000'0000'0000,
			PhysicsAfterDeform   = 0b0001'0000'0000'0000,
			ExternalParentDeform = 0b0010'0000'0000'0000,
		};
		struct IKLink
		{
			pmx_index_t Bone;
			struct
			{
				Vector4 Min, Max;
			} AngleLimits;
			bool IsLimited;
		};
		Bone() {}
		std::u32string NameLocal, NameUniversal;
		Vector4 Position;
		pmx_index_t ParentIndex;
		int32_t Layer;
		Flag Flags;
		union
		{
			// Depends on whether [IndexedTail] flag was on.
			Vector4 TailPosition{};
			pmx_index_t TailIndex;
		};
		struct
		{
			pmx_index_t Parent;
			float Influence;
		} InheritControl;
		struct
		{
			Vector4 Direction;
		} FixedAxis;
		struct
		{
			Vector4 X, Z;
		} LocalCoordinate;
		struct
		{
			pmx_index_t Parent;
		} ExternalParant;
		struct
		{
			pmx_index_t Target;
			int32_t LoopCount;
			float LimitRad;
			int32_t LinkCount;
			Array<IKLink> Links;
		} IK;
	};
	struct Morph
	{
		enum MorphType
		{
			GroupMorph    = 0,
			VertexMorph   = 1,
			BoneMorph     = 2,
			UVMorph       = 3,
			UVExt1Morph   = 4,
			UVExt2Morph   = 5,
			UVExt3Morph   = 6,
			UVExt4Morph   = 7,
			MaterialMorph = 8,
			FlipMorph     = 9,
			ImpulseMorph  = 10,
		};
		struct MorphOffset
		{
			MorphOffset() {}
			pmx_index_t Index;
			union
			{
				struct { float Influence; } Group, Flip;
				struct { Vector4 Translation; } Vertex;
				struct { Vector4 Translation, Rotation; } Bone;
				struct { Vector4 Floats; } UV;
				struct
				{
					Vector4 Diffuse, Specular, Ambient, EdgeColor, TextureTint, EnvironmentTint, ToonTint;
					float Specularity, EdgeSize;
					int8_t Unknown;
				} Material;
				struct {
					Vector4 LinearVelocity, AngularVelocity;
					int8_t Flags;
				} Impulse;
			};
		};

		std::u32string
			NameLocal,
			NameUniversal;
		int_fast8_t Panel; // A handle-like value.
		MorphType Type;
		Array<MorphOffset> Offsets;
	};
	struct DisplayFrame
	{
		enum DisplayFrameType
		{
			BoneFrame  = 0,
			MorphFrame = 1,
		};
		struct Frame
		{
			DisplayFrameType Type;
			pmx_index_t Index;
		};
		std::u32string
			NameLocal,
			NameUniversal;
		uint8_t Flags;
		Array<Frame> Frames;
	};
	struct RigidBody
	{
		enum RigidBodyShape
		{
			Sphere  = 0,
			Box     = 1,
			Capsule = 2,
		};
		enum RigidBodyPhysics
		{
			BoneBased    = 0,
			PhysicsBased = 1,
			Both         = 2,
		};

		std::u32string
			NameLocal,
			NameUniversal;
		pmx_index_t OwnerBone;
		uint8_t Group;
		uint16_t NonCollisionGroup;
		RigidBodyShape Shape;
		Vector4 Size, Position, Rotation;
		float Mass, Attenuation, RotationDamping,
			Repulsion, FrictionalForce;
		RigidBodyPhysics Physics;
	};
	struct Joint
	{
		enum JointType
		{
			SpringSixDoF = 0,
			SixDoF       = 1,
			P2P          = 2,
			ConeTwist    = 3,
			Slider       = 4,
			Hinge        = 5,
		};
		std::u32string
			NameLocal,
			NameUniversal;
		JointType Type;
		pmx_index_t IndexA, IndexB;
		Vector4 Position, Rotation,
			MinPosition, MaxPosition,
			MinRotation, MaxRotation,
			PositionSpring, RotationSpring;
	};

	float Version;
	int AttachedVectorCount;
	std::u32string
		ModelNameLocal,
		ModelNameUniversal,
		CommentNameLocal,
		CommentNameUniversal;
	Array<Vertex> Vertices;
	Array<int32_t> Indices;
	Array<std::u32string> Textures;
	Array<Material> Materials;
	Array<Bone> Bones;
	Array<Morph> Morphs;
	Array<DisplayFrame> DisplayFrames;
	Array<RigidBody> RigidBodies;
	Array<Joint> Joints;

	static PmxResolutionError
		TryResolve(const void* raw, size_t size, Pmx& out);
	static Pmx
		Resolve(const void* raw, size_t size);
};
