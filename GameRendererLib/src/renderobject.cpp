#include <stdafx.h>

RenderObject::RenderObject()
	: activeFlag(true)
	, VAO(-1)
	, VBO(-1)
	, shaderAttributesTotalSize(0)
	, bufferVerticesCount(0)
{}

RenderObject::RenderObject(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
	: activeFlag(true)
	, VAO(-1)
	, VBO(-1)
	, shaderAttributesTotalSize(0)
	, bufferVerticesCount(0)
{
	loadShader(vertexShaderFilename, fragmentShaderFilename);
}

void RenderObject::setActive(bool status)
{
	activeFlag = status;
}

bool RenderObject::isActive()const
{
	return activeFlag;
}

void RenderObject::deepCopy(const RenderObject& object)
{
	this->shader = object.shader;
	this->VAO = object.VAO;
	this->VBO = object.VBO;
	this->bufferVerticesCount = object.bufferVerticesCount;
	this->uniforms = object.uniforms;
	this->shaderAttributes = object.shaderAttributes;
	this->shaderAttributesTotalSize = object.shaderAttributesTotalSize;
}

void RenderObject::loadShader(std::shared_ptr<Shader> shader)
{
	this->shader = shader;
}

void RenderObject::loadShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	shader = std::make_shared<Shader>("RenderObjectShader", vertexShaderFilename, fragmentShaderFilename);

	if (shader->getErrorCode() != Shader::NO_ERROR)
		errorCode.push(SHADER_CREATE_ERROR);
}

void RenderObject::addUniform(std::shared_ptr<UniformData> uniform)
{
	uniforms.insert(UniformMap::value_type(uniform->name, uniform));
}

void RenderObject::deleteUniform(const std::string& name)
{
	uniforms.erase(name);
}

UniformDataPtr RenderObject::getUniform(const std::string& name)
{
	auto uniform = uniforms.find(name);
	if (uniform != uniforms.end())
		return uniform->second;
	else return std::make_shared<NullUniformData>();
}

void RenderObject::initBuffersAndArrays()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		errorCode.push(BUFFER_CREATE_ERROR);
}

void RenderObject::loadUniformsLoc()
{
	if (shader->getErrorCode() == Shader::NO_ERROR)
	{
		for(UniformMap::iterator it = uniforms.begin(); it != uniforms.end(); ++it)
		{
			it->second->loadLocation(shader->getProgram());

			if (it->second->location < 0)
				errorCode.push(UNIFORM_NOT_FOUND);
		}
	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void RenderObject::addShaderAttribute(ShaderAttribute attribute)
{
	shaderAttributes.push_back(attribute);
	shaderAttributesTotalSize += attribute.size;
}

void RenderObject::loadBuffer(const std::vector<float>& verticesBuffer)
{
	GLenum err;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesBuffer.size() * sizeof(float), verticesBuffer.data(), GL_STATIC_DRAW);

	err = glGetError();
	if (err != GL_NO_ERROR)
		errorCode.push(BUFFER_DATA_ERROR);

	initAttribArrayPointers();

	bufferVerticesCount = verticesBuffer.size() / shaderAttributesTotalSize;
}

void RenderObject::initAttribArrayPointers()
{
	for (ShaderAttrList::value_type shaderAttrEntry : shaderAttributes)
	{
		glVertexAttribPointer(shaderAttrEntry.location, shaderAttrEntry.size, GL_FLOAT, GL_FALSE, shaderAttributesTotalSize * sizeof(float), shaderAttrEntry.offset);
		glEnableVertexAttribArray(shaderAttrEntry.location);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			errorCode.push(VERTEX_ATTRIB_POINTER_NOT_VALID);
	}
}

void RenderObject::init()
{
	if (shader)
	{
		initBuffersAndArrays();
		loadUniformsLoc();
	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void RenderObject::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR && activeFlag)
	{
		bindVertexArray();

		updateUniforms();

		drawArrays();
	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void RenderObject::bindVertexArray()
{
	glUseProgram(shader->getProgram());
	glBindVertexArray(VAO);
	glBindBuffer(GL_VERTEX_ARRAY, VBO);
}

void RenderObject::updateUniforms()
{
	std::for_each(uniforms.begin(), uniforms.end(), [](auto uniformEntry) { 
		uniformEntry.second->update(); 
	});
}

void RenderObject::drawArrays()
{
	glDrawArrays(GL_TRIANGLES, 0, bufferVerticesCount);
}

void RenderObject::invalidate()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

RenderObject::error RenderObject::getErrorCode()
{
	if (!errorCode.empty())
	{
		error code = errorCode.top();
		errorCode.pop();
		return code;
	}
	else return NO_ERROR;
}