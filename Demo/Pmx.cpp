// File: Pmx.hpp
// Author: Rendong Liang (Liong)
#include "Pmx.hpp"
#include "LiongPlus/MemoryReader.hpp"
#include "LiongPlus/Serialization/Unicode.hpp"

using namespace std;
using namespace LiongPlus;
using namespace LiongPlus::Serialization;

using PmxResolutionError = Pmx::PmxResolutionError;

using pmx_index_t = int32_t;
using pmx_size_t = int32_t;

enum GlobalPropertyKey
{
	TextEncoding = 0,
	AttachedVectorCount = 1,
	VertexIndexSize = 2,
	TextureIndexSize = 3,
	MaterialIndexSize = 4,
	BoneIndexSize = 5,
	MorphIndexSize = 6,
	RigidBodyIndexSize = 7,
};
	
struct PmxReader
{
public:
	MemoryReader Reader;
	struct PmxHeader
	{
		float Version;
		Array<int32_t> Globals;
		std::u32string
			ModelNameLocal,
			ModelNameUniversal,
			CommentNameLocal,
			CommentNameUniversal;
	} Header;

	//
	// Atomic parsing operations.
	//

	template<typename T, typename TOut>
	PmxResolutionError
		Read(TOut& out)
	{
		if (!Reader.TryRead<T>(out))
			return Pmx::Error_UnexpectedEof;
		return Pmx::Success_Perfect;
	}
	template<GlobalPropertyKey TSizeKey>
	PmxResolutionError
		ReadIndex(int& out)
	{
		static_assert(
			TSizeKey == TextureIndexSize ||
			TSizeKey == MaterialIndexSize ||
			TSizeKey == BoneIndexSize ||
			TSizeKey == MorphIndexSize ||
			TSizeKey == RigidBodyIndexSize,
			"Invalid index size key");
		switch (Header.Globals[TSizeKey])
		{
		case 1:
			if (!Reader.TryRead<int8_t>(out))
				return Pmx::Error_UnexpectedEof;
			break;
		case 2:
			if (!Reader.TryRead<int16_t>(out))
				return Pmx::Error_UnexpectedEof;
			break;
		case 4:
			if (!Reader.TryRead<int32_t>(out))
				return Pmx::Error_UnexpectedEof;
			break;
		default:
			return Pmx::Error_UnsupportedIndexSize;
		}
		return Pmx::Success_Perfect;
	}
	template<>
	PmxResolutionError
		ReadIndex<VertexIndexSize>(int& out)
	{
		switch (Header.Globals[VertexIndexSize])
		{
		case 1:
			if (!Reader.TryRead<uint8_t>(out))
				return Pmx::Error_UnexpectedEof;
			break;
		case 2:
			if (!Reader.TryRead<uint16_t>(out))
				return Pmx::Error_UnexpectedEof;
			break;
		case 4:
			if (!Reader.TryRead<int32_t>(out))
				return Pmx::Error_UnexpectedEof;
			break;
		default:
			return Pmx::Error_UnsupportedIndexSize;
		}
		return Pmx::Success_Perfect;
	}
	template<size_t TVecLen>
	PmxResolutionError
		ReadVector(Vector4& out)
	{
		if (!Reader.TryReadManyByIndex<float>(0, TVecLen, out))
			return Pmx::Error_UnexpectedEof;
		return Pmx::Success_Perfect;
	}

	/*
	 * This function is also atomic but [Error_TextDecoding] might
	 *   be returned when the encoding of incoming text is nor
	 *   recognizable.
	 * Error checking should be done before another read is called
	 *   if another read will return an error code to the same
	 *   receiver variable.
	 */
	PmxResolutionError
		ReadText(std::u32string& out)
	{
		size_t strLen;
		if (!Reader.TryRead<int32_t>(strLen))
			return Pmx::Error_UnexpectedEof;
		// Decode to unicode.
		if (Header.Globals[TextEncoding] == 0) // UTF-16LE
		{
			if (Unicode::TryFromUtf16((const uint16_t*)Reader, (const uint16_t*)Reader + strLen / 2, out).IsError())
				return Pmx::Error_TextDecoding;
		}
		else // UTF-8
		{
			if (Unicode::TryFromUtf8((const uint8_t*)Reader, (const uint8_t*)Reader + strLen, out).IsError())
				return Pmx::Error_TextDecoding;
		}
		Reader.Skip(strLen);
		return Pmx::Success_Perfect;
	}
};


//
// Header.
//
PmxResolutionError
	_aux_ReadHeader(PmxReader& reader)
{
	PmxResolutionError err = Pmx::Success_Perfect;
	// Check signature.
	auto sig = (const uint8_t*)reader.Reader;
	if ('P' != sig[0] || 'M' != sig[1] ||
		'X' != sig[2] || ' ' != sig[3])
		return Pmx::Error_NotAcknowkedged;
	reader.Reader.Skip(4);

	// Read version number.
	err = reader.Read<float>(reader.Header.Version);
	if (err.IsError()) return err;

	// Load globals.
	pmx_size_t globalCount;
	err = reader.Read<int8_t>(globalCount);
	if (err.IsError()) return err;
	reader.Header.Globals = Array<int32_t>(globalCount);
	// There should be at least 8 properties for MMD 2.0 and 2.1.
	if (reader.Header.Version <= 2.1 && globalCount < 8)
		return Pmx::Error_UnsupportedGlobals;
	for (size_t i = 0; i < globalCount; ++i)
		err = reader.Read<int8_t>(reader.Header.Globals[i]);
	if (err.IsError()) return err;
	err =
		reader.ReadText(reader.Header.ModelNameLocal),
		reader.ReadText(reader.Header.ModelNameUniversal),
		reader.ReadText(reader.Header.CommentNameLocal),
		reader.ReadText(reader.Header.CommentNameUniversal);
	return err;
}
	
//
// Vertex and deform.
//

using DeformData = decltype(Pmx::Vertex::DeformData);
using DeformType = decltype(Pmx::Vertex::DeformType);


PmxResolutionError
	_aux_MakeBDef1(PmxReader& reader, DeformData& out)
{
	return reader.ReadIndex<BoneIndexSize>(out.Bone1Index);
}
PmxResolutionError
	_aux_MakeBDef2(PmxReader& reader, DeformData& out)
{
	reader.ReadIndex<BoneIndexSize>(out.Bone1Index);
	reader.ReadIndex<BoneIndexSize>(out.Bone2Index);
	return reader.Read<float>(out.Bone1Weight);
}
PmxResolutionError
	_aux_MakeBDef4OrQDef(PmxReader& reader, DeformData& out)
{
	reader.ReadIndex<BoneIndexSize>(out.Bone1Index);
	reader.ReadIndex<BoneIndexSize>(out.Bone2Index);
	reader.ReadIndex<BoneIndexSize>(out.Bone3Index);
	reader.ReadIndex<BoneIndexSize>(out.Bone4Index);
	reader.Read<float>(out.Bone1Weight);
	reader.Read<float>(out.Bone2Weight);
	reader.Read<float>(out.Bone3Weight);
	return reader.Read<float>(out.Bone4Weight);
}
PmxResolutionError
	_aux_MakeSDef(PmxReader& reader, DeformData& out)
{
	reader.ReadIndex<BoneIndexSize>(out.Bone1Index);
	reader.ReadIndex<BoneIndexSize>(out.Bone2Index);
	reader.Read<float>(out.Bone1Weight);
	reader.ReadVector<3>(out.C);
	reader.ReadVector<3>(out.R0);
	return reader.ReadVector<3>(out.R1);
}
PmxResolutionError
	_aux_MakeDeformData(PmxReader& reader, DeformType type,
		DeformData& out)
{
	switch (type)
	{
	case DeformType::BDef1:
		return _aux_MakeBDef1(reader, out);
	case DeformType::BDef2:
		return _aux_MakeBDef2(reader, out);
	case DeformType::BDef4:
	case DeformType::QDef:
		return _aux_MakeBDef4OrQDef(reader, out);
	case DeformType::SDef:
		return _aux_MakeSDef(reader, out);
	}
	return Pmx::Error_UnsupportedDeform;
}
PmxResolutionError
	_aux_ReadVertex(PmxReader& reader, Pmx::Vertex& out)
{
	reader.ReadVector<3>(out.Position),
	reader.ReadVector<3>(out.Normal),
	reader.ReadVector<2>(out.UV);
		
	const auto posAttached = reader.Header.Globals[AttachedVectorCount];
	out.AttachedVectors = Array<Vector4>(posAttached);
	for (size_t i = 0; i < posAttached; ++i)
		reader.ReadVector<4>(out.AttachedVectors[i]);

	reader.Read<int8_t>(out.DeformType);
	_aux_MakeDeformData(reader, out.DeformType, out.DeformData);
	return reader.Read<float>(out.EdgeScale);
}
PmxResolutionError
	_aux_ReadVertices(PmxReader& reader, Array<Pmx::Vertex>& out)
{
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;

	out = Array<Pmx::Vertex>(count);
	for (auto& ref : out)
	{
		err = _aux_ReadVertex(reader, ref);
		if (err.IsError()) return err;
	}
	return err;
}

//
// Vertex indices.
//
PmxResolutionError
	_aux_ReadIndices(PmxReader& reader, Array<int32_t>& out)
{
	PmxResolutionError err = Pmx::Success_Perfect;
	pmx_size_t count;
	if (err.IsError()) return err;
	err = reader.Read<int32_t>(count);
		
	out = Array<int32_t>(count);
	switch (reader.Header.Globals[VertexIndexSize])
	{
	case 1:
		err =
			reader.Reader.TryReadMany<uint8_t>(
				count, out.begin()) ?
			Pmx::Success_Perfect : Pmx::Error_UnexpectedEof;
		break;
	case 2:
		err =
			reader.Reader.TryReadMany<uint16_t>(
				count, out.begin()) ?
			Pmx::Success_Perfect : Pmx::Error_UnexpectedEof;
		break;
	case 4:
		err =
			reader.Reader.TryReadMany<uint32_t>(
				count, out.begin()) ?
			Pmx::Success_Perfect : Pmx::Error_UnexpectedEof;
		break;
	default:
		return Pmx::Error_UnsupportedIndexSize;
	}
	return err;
}

//
// Textures.
//
PmxResolutionError
	_aux_ReadTextures(PmxReader& reader, Array<std::u32string>& out)
{
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;

	out = Array<std::u32string>(count);
	for (auto& ref : out)
	{
		err = reader.ReadText(ref);
		if (err.IsError()) return err;
	}
	return err;
}

//
// Material.
//
PmxResolutionError
	_aux_ReadMaterials(PmxReader& reader, Array<Pmx::Material>& out)
{
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;

	out = Array<Pmx::Material>(count);
	for (auto& ref : out)
	{
		reader.ReadText(ref.NameLocal);
		reader.ReadText(ref.NameUniversal);
		reader.ReadVector<4>(ref.Diffuse);
		reader.ReadVector<3>(ref.Specular);
		reader.Read<float>(ref.SpecularStrength);
		reader.ReadVector<3>(ref.Ambient);
		reader.Read<int8_t>(ref.Flags);
		reader.ReadVector<4>(ref.Edge);
		reader.Read<float>(ref.EdgeScale);
		reader.ReadIndex<TextureIndexSize>(ref.TexIndex);
		reader.ReadIndex<TextureIndexSize>(ref.EnvIndex);
		reader.Read<int8_t>(ref.EnvBlendMode);
		reader.Read<int8_t>(ref.ToonRef);
		if (ref.ToonRef == Pmx::Material::TextureReference)
			reader.ReadIndex<TextureIndexSize>(ref.ToonValue);
		else // InternalReference
			reader.Read<int8_t>(ref.ToonValue);
		reader.ReadText(ref.Metadata);
		err = reader.Read<int32_t>(ref.VertexCount);
		if (err.IsError()) return err;
	}
	return err;
}

//
// Bones
//
PmxResolutionError
	_aux_ReadBone(PmxReader& reader, Pmx::Bone& out)
{
	reader.ReadText(out.NameLocal);
	reader.ReadText(out.NameUniversal);
	reader.ReadVector<3>(out.Position);
	reader.ReadIndex<BoneIndexSize>(out.ParentIndex);
	reader.Read<int32_t>(out.Layer);
	reader.Read<int16_t>(out.Flags);
	auto err = out.Flags & Pmx::Bone::IndexedTail ?
		reader.ReadIndex<BoneIndexSize>(out.TailIndex) :
		reader.ReadVector<3>(out.TailPosition);

	// Inherit bone.
	if ((out.Flags & Pmx::Bone::InheritRotation) ||
		(out.Flags & Pmx::Bone::InheritTranslation))
	{
		reader.ReadIndex<BoneIndexSize>(out.InheritControl.Parent);
		err = reader.Read<float>(out.InheritControl.Influence);
	}
	if (out.Flags & Pmx::Bone::HasFixedAxis)
		err = reader.ReadVector<3>(out.FixedAxis.Direction);
	if (out.Flags & Pmx::Bone::HasLocalCoordinate)
	{
		reader.ReadVector<3>(out.LocalCoordinate.X);
		err = reader.ReadVector<3>(out.LocalCoordinate.Z);
	}
	if (out.Flags & Pmx::Bone::ExternalParentDeform)
		err = reader.ReadIndex<BoneIndexSize>(out.ExternalParant.Parent);
	if (out.Flags & Pmx::Bone::IsIKEnabled)
	{
		reader.ReadIndex<BoneIndexSize>(out.IK.Target);
		reader.Read<int32_t>(out.IK.LoopCount);
		reader.Read<float>(out.IK.LimitRad);
		err = reader.Read<int32_t>(out.IK.LinkCount);
		if (err.IsError()) return err;

		out.IK.Links = Array<Pmx::Bone::IKLink>(out.IK.LinkCount);
		for (auto& link : out.IK.Links)
		{
			reader.ReadIndex<BoneIndexSize>(link.Bone);
			err = reader.Read<int8_t>(link.IsLimited);
			if (link.IsLimited)
			{
				reader.ReadVector<3>(link.AngleLimits.Min);
				err = reader.ReadVector<3>(link.AngleLimits.Max);
				if (err.IsError()) return err;
			}
			if (err.IsError()) return err;
		}
	}

	return err;
}
PmxResolutionError
	_aux_ReadBones(PmxReader& reader, Array<Pmx::Bone>& out)
{
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;

	out = Array<Pmx::Bone>(count);
	for (auto& ref : out)
	{
		err = _aux_ReadBone(reader, ref);
		if (err.IsError()) return err;
	}
	return err;
}

//
// Morphs.
//
PmxResolutionError
	_aux_ReadMorph(PmxReader& reader, Pmx::Morph& out)
{
	reader.ReadText(out.NameLocal);
	reader.ReadText(out.NameUniversal);
	reader.Read<int8_t>(out.Panel);
	reader.Read<int8_t>(out.Type);
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;
	out.Offsets = Array<Pmx::Morph::MorphOffset>(count);

	for (auto& offset : out.Offsets)
	{
		switch (out.Type)
		{
		case Pmx::Morph::GroupMorph:
			reader.ReadIndex<MorphIndexSize>(offset.Index);
			err = reader.Read<float>(offset.Group.Influence);
			break;
		case Pmx::Morph::VertexMorph:
			reader.ReadIndex<VertexIndexSize>(offset.Index);
			err = reader.ReadVector<3>(offset.Vertex.Translation);
			break;
		case Pmx::Morph::BoneMorph:
			reader.ReadIndex<BoneIndexSize>(offset.Index);
			reader.ReadVector<3>(offset.Bone.Translation);
			err = reader.ReadVector<4>(offset.Bone.Rotation);
			break;
		case Pmx::Morph::UVMorph:
		case Pmx::Morph::UVExt1Morph:
		case Pmx::Morph::UVExt2Morph:
		case Pmx::Morph::UVExt3Morph:
		case Pmx::Morph::UVExt4Morph:
			reader.ReadIndex<VertexIndexSize>(offset.Index);
			err = reader.ReadVector<4>(offset.UV.Floats);
			break;
		case Pmx::Morph::MaterialMorph:
			reader.ReadIndex<MaterialIndexSize>(offset.Index);
			reader.Read<int8_t>(offset.Material.Unknown);
			reader.ReadVector<4>(offset.Material.Diffuse);
			reader.ReadVector<3>(offset.Material.Specular);
			reader.Read<float>(offset.Material.Specularity);
			reader.ReadVector<3>(offset.Material.Ambient);
			reader.ReadVector<4>(offset.Material.EdgeColor);
			reader.Read<float>(offset.Material.EdgeSize);
			reader.ReadVector<4>(offset.Material.TextureTint);
			reader.ReadVector<4>(offset.Material.EnvironmentTint);
			err = reader.ReadVector<4>(offset.Material.ToonTint);
			break;
		case Pmx::Morph::FlipMorph:
			reader.ReadIndex<MorphIndexSize>(offset.Index);
			err = reader.Read<float>(offset.Group.Influence);
			break;
		case Pmx::Morph::ImpulseMorph:
			reader.ReadIndex<RigidBodyIndexSize>(offset.Index);
			reader.Read<int8_t>(offset.Impulse.Flags);
			reader.ReadVector<3>(offset.Impulse.LinearVelocity);
			err = reader.ReadVector<3>(offset.Impulse.AngularVelocity);
			break;
		default:
			return Pmx::Error_UnsupportedMorph;
		}
		if (err.IsError()) return err;
	}

	return err;
}
PmxResolutionError
	_aux_ReadMorphs(PmxReader& reader, Array<Pmx::Morph>& out)
{
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;

	out = Array<Pmx::Morph>(count);
	for (auto& ref : out)
	{
		err = _aux_ReadMorph(reader, ref);
		if (err.IsError()) return err;
	}
	return err;
}

//
// Display frames.
//
PmxResolutionError
	_aux_ReadDisplayFrame(PmxReader& reader, Pmx::DisplayFrame& out)
{
	reader.ReadText(out.NameLocal);
	reader.ReadText(out.NameUniversal);
	reader.Read<int8_t>(out.Flags);
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;

	out.Frames = Array<Pmx::DisplayFrame::Frame>(count);
	for (auto& frame : out.Frames)
	{
		reader.Read<int8_t>(frame.Type);
		err = (Pmx::DisplayFrame::BoneFrame == frame.Type ?
			reader.ReadIndex<BoneIndexSize>(frame.Index) :
			reader.ReadIndex<MorphIndexSize>(frame.Index));
		if (err.IsError()) return err;
	}

	return err;
}
PmxResolutionError
	_aux_ReadDisplayFrames(PmxReader& reader, Array<Pmx::DisplayFrame>& out)
{
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;

	out = Array<Pmx::DisplayFrame>(count);
	for (auto& ref : out)
	{
		err = _aux_ReadDisplayFrame(reader, ref);
		if (err.IsError()) return err;
	}
	return err;
}

//
// RigidBodies.
//
PmxResolutionError
	_aux_ReadRigidBody(PmxReader& reader, Pmx::RigidBody& out)
{
	reader.ReadText(out.NameLocal);
	reader.ReadText(out.NameUniversal);
	reader.ReadIndex<BoneIndexSize>(out.OwnerBone);
	reader.Read<int8_t>(out.Group);
	reader.Read<int16_t>(out.NonCollisionGroup);
	reader.Read<int8_t>(out.Shape);
	reader.ReadVector<3>(out.Size);
	reader.ReadVector<3>(out.Position);
	reader.ReadVector<3>(out.Rotation);
	reader.Read<float>(out.Mass);
	reader.Read<float>(out.Attenuation);
	reader.Read<float>(out.RotationDamping);
	reader.Read<float>(out.Repulsion);
	reader.Read<float>(out.FrictionalForce);
	return reader.Read<int8_t>(out.Physics);

}
PmxResolutionError
	_aux_ReadRigidBodies(PmxReader& reader, Array<Pmx::RigidBody>& out)
{
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;
	out = Array<Pmx::RigidBody>(count);

	for (auto& ref : out)
	{
		err = _aux_ReadRigidBody(reader, ref);
		if (err.IsError()) return err;
	}
	return err;
}

//
// Joints.
//
PmxResolutionError
	_aux_ReadJoint(PmxReader& reader, Pmx::Joint& out)
{
	reader.ReadText(out.NameLocal);
	reader.ReadText(out.NameUniversal);
	reader.Read<int8_t>(out.Type);
	reader.ReadIndex<RigidBodyIndexSize>(out.IndexA);
	reader.ReadIndex<RigidBodyIndexSize>(out.IndexB);
	reader.ReadVector<3>(out.Position);
	reader.ReadVector<3>(out.Rotation);
	reader.ReadVector<3>(out.MinPosition);
	reader.ReadVector<3>(out.MaxPosition);
	reader.ReadVector<3>(out.MinRotation);
	reader.ReadVector<3>(out.MaxRotation);
	reader.ReadVector<3>(out.PositionSpring);
	return reader.ReadVector<3>(out.RotationSpring);
}
PmxResolutionError
	_aux_ReadJoints(PmxReader& reader, Array<Pmx::Joint>& out)
{
	pmx_size_t count;
	auto err = reader.Read<int32_t>(count);
	if (err.IsError()) return err;
	out = Array<Pmx::Joint>(count);

	for (auto& ref : out)
	{
		err = _aux_ReadJoint(reader, ref);
		if (err.IsError()) return err;
	}
	return err;
}

//
// Resolution.
//
PmxResolutionError
Pmx::TryResolve(const void* raw, size_t size, Pmx& out)
{
	PmxReader reader{ {raw, size}, {} };

	PmxResolutionError err = Success_Perfect;

	// 25 is the least possible size for PMX header.
	if (size < 25) return Error_NotAcknowkedged;
	err = _aux_ReadHeader(reader);
	if (err.IsError()) return err;

	out.Version = reader.Header.Version;
	out.AttachedVectorCount = reader.Header.Globals[GlobalPropertyKey::AttachedVectorCount];
	out.ModelNameLocal = reader.Header.ModelNameLocal;
	out.ModelNameUniversal = reader.Header.ModelNameUniversal;
	out.CommentNameLocal = reader.Header.CommentNameLocal;
	out.CommentNameUniversal = reader.Header.CommentNameUniversal;

	err = _aux_ReadVertices(reader, out.Vertices);
	if (err.IsError()) return err;
	err = _aux_ReadIndices(reader, out.Indices);
	if (err.IsError()) return err;
	err = _aux_ReadTextures(reader, out.Textures);
	if (err.IsError()) return err;
	err = _aux_ReadMaterials(reader, out.Materials);
	if (err.IsError()) return err;
	err = _aux_ReadBones(reader, out.Bones);
	if (err.IsError()) return err;
	err = _aux_ReadMorphs(reader, out.Morphs);
	if (err.IsError()) return err;
	err = _aux_ReadDisplayFrames(reader, out.DisplayFrames);
	if (err.IsError()) return err;
	err = _aux_ReadRigidBodies(reader, out.RigidBodies);
	if (err.IsError()) return err;
	err = _aux_ReadJoints(reader, out.Joints);
	if (err.IsError()) return err;

	return err;
}

Pmx
Pmx::Resolve(const void* raw, size_t size)
{
	Pmx rv;
	if (TryResolve(raw, size, rv).IsError())
		throw std::runtime_error("Unable to resolve PMX file.");
	return rv;
}
	