// File: VertexArray.cpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../../Array.hpp"
#include "BufferObject.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

struct VertexArrayAttribute
{
	unsigned VertexBufferHandle;
	unsigned AttributeIndex;
	unsigned VectorLength;
	unsigned Offset;
	unsigned Stride;
};

struct DrawArraysIndirectCommand
{
public:
	uint32_t VertexCount;
	uint32_t InstanceCount;
	uint32_t FirstVertex;
	uint32_t FirstInstance;
};
struct DrawElementsIndirectCommand
{
public:
	uint32_t IndexCount;
	uint32_t InstanceCount;
	uint32_t FirstIndex;
	int32_t FirstVertex;
	uint32_t FirstInstance;
};

class VertexArray
{
private:
	unsigned _Handle;

	void _Clear();

public:
	class ActivationGuard
	{
	private:
		bool _Flag;

	public:
		ActivationGuard(unsigned handle);
		ActivationGuard(ActivationGuard &&);
		~ActivationGuard();
	};

	VertexArray();
	VertexArray(VertexArray&&);
	~VertexArray();

	VertexArray& operator=(VertexArray&&);

	void Draw(unsigned mode,
		unsigned firstVertex = 0, unsigned vertexCount = 1);
	void DrawInstanced(unsigned mode,
		unsigned firstVertex, unsigned VertexCount,
		unsigned instCount);

	void EnableAttributes(unsigned count);
	void DisableAttributes(unsigned count);

	void SetAttributes(const Array<VertexArrayAttribute>& attrs);

	static void MultiDrawArrays(unsigned mode,
		const Array<DrawArraysIndirectCommand> cmds);
	static void MultiDrawElements(unsigned mode,
		const Array<DrawElementsIndirectCommand> cmds);

	ActivationGuard Activate() const;
};

_L_NS_END_GRAPHICS_OPENGL
