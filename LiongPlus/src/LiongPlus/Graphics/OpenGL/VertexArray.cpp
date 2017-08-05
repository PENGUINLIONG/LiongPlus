// File: VertexArray.cpp
// Author: Rendong Liang (Liong)
#include <utility>
#include "LiongPlus/Graphics/OpenGL/VertexArray.hpp"
#include "LiongPlus/Platform/MinOpenGL.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

using namespace Platform::OpenGL;

VertexArray::VertexArray() : _Handle()
{
	if (glCreateVertexArrays)
		glCreateVertexArrays(1, &_Handle);
	else
		glGenVertexArrays(1, &_Handle);
}
VertexArray::VertexArray(VertexArray&& inst) :
	_Handle(std::exchange(inst._Handle, 0))
{
}
VertexArray::~VertexArray()
{
	_Clear();
}

VertexArray& VertexArray::operator=(VertexArray&& inst)
{
	_Clear();
	_Handle = std::exchange(inst._Handle, 0);
	return *this;
}

void VertexArray::_Clear()
{
	if (_Handle)
		glDeleteVertexArrays(1, &_Handle);
}

void VertexArray::Draw(unsigned mode,
	unsigned firstVertex, unsigned vertexCount)
{
	auto act = Activate();
	glDrawArrays(mode, firstVertex, vertexCount);
}
void VertexArray::DrawInstanced(unsigned mode,
	unsigned firstVertex, unsigned vertexCount,
	unsigned instCount)
{
	auto act = Activate();
	glDrawArraysInstanced(mode, firstVertex, vertexCount, instCount);
}

void VertexArray::EnableAttributes(unsigned count)
{
	if (glEnableVertexArrayAttrib)
	{
		for (auto i = 0; i < count; ++i)
			glEnableVertexArrayAttrib(_Handle, i);
	}
	else
	{
		glBindVertexArray(_Handle);
		for (auto i = 0; i < count; ++i)
			glEnableVertexAttribArray(i);
		glBindVertexArray(0);
	}
}
void VertexArray::DisableAttributes(unsigned count)
{

	if (glDisableVertexArrayAttrib)
	{
		for (auto i = 0; i < count; ++i)
			glDisableVertexArrayAttrib(_Handle, i);
	}
	else
	{
		glBindVertexArray(_Handle);
		for (auto i = 0; i < count; ++i)
			glDisableVertexAttribArray(i);
		glBindVertexArray(0);
	}
}

void VertexArray::SetAttributes(const Array<VertexArrayAttribute>& attrs)
{
	if (glVertexArrayAttribFormat)
	{
		for (const auto& attr : attrs)
		{
			glVertexArrayAttribFormat(_Handle, attr.AttributeIndex, attr.VectorLength, GL_FLOAT, false, 0);
			glVertexArrayAttribBinding(_Handle, attr.AttributeIndex, attr.AttributeIndex);
			glVertexArrayVertexBuffer(_Handle, attr.AttributeIndex, attr.VertexBufferHandle, attr.Offset, attr.Stride);
		}
	}
	else
	{
		glBindVertexArray(_Handle);
		if (glVertexAttribFormat)
		{
			for (auto& attr : attrs)
			{
				glVertexAttribFormat(attr.AttributeIndex, attr.VectorLength, GL_FLOAT, false, 0);
				glVertexAttribBinding(attr.AttributeIndex, attr.AttributeIndex);
				glBindVertexBuffer(attr.AttributeIndex, attr.VertexBufferHandle, attr.Offset, attr.Stride);
			}
		}
		else
		{
			for (auto& attr : attrs)
			{
				glBindBuffer(GL_ARRAY_BUFFER, attr.VertexBufferHandle);
				glVertexAttribPointer(attr.AttributeIndex, attr.VectorLength, GL_FLOAT, false, attr.Stride, (char*)(nullptr) + attr.Offset);
			}
			// Not a mistake.
			// It's places outside for less useless bindings.
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
	}
}

void VertexArray::MultiDrawArrays(unsigned mode,
	const Array<DrawArraysIndirectCommand> cmds)
{
	if (glMultiDrawArraysIndirect)
		glMultiDrawArraysIndirect(mode, cmds.data(), cmds.size(), 0);
	else if (glDrawArraysIndirect)
	{
		for (const auto& cmd : cmds)
			glDrawArraysIndirect(mode, &cmd);
	}
	else
	{
		for (const auto& cmd : cmds)
			glDrawArraysInstanced(mode, cmd.FirstVertex, cmd.VertexCount, cmd.InstanceCount);
	}
}
void VertexArray::MultiDrawElements(unsigned mode,
	const Array<DrawElementsIndirectCommand> cmds)
{
	if (glMultiDrawElementsIndirect)
		glMultiDrawElementsIndirect(mode, GL_UNSIGNED_INT, cmds.data(), cmds.size(), 0);
	else if (glDrawElementsIndirect)
	{
		for (const auto& cmd : cmds)
			glDrawElementsIndirect(mode, GL_UNSIGNED_INT, &cmd);
	}
	else
	{
		for (const auto& cmd : cmds)
			glDrawElementsInstancedBaseVertexBaseInstance(mode,
				cmd.IndexCount, GL_UNSIGNED_INT,
				(char*)(nullptr) + cmd.FirstIndex * 4,
				(int)cmd.InstanceCount,
				cmd.FirstVertex, cmd.FirstInstance);
	}
}

VertexArray::ActivationGuard VertexArray::Activate() const
{
	return { _Handle };
}



VertexArray::ActivationGuard::ActivationGuard(unsigned handle) :
	_Flag(true)
{
	glBindVertexArray(handle);
}
VertexArray::ActivationGuard::ActivationGuard(ActivationGuard&& inst) :
	_Flag(std::exchange(inst._Flag, false))
{
}
VertexArray::ActivationGuard::~ActivationGuard()
{
	if (_Flag)
		glBindVertexArray(0);
}

_L_NS_END_GRAPHICS_OPENGL
