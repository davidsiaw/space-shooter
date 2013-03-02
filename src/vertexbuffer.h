#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "SDL.h"
#include "GL/glew.h"
#include "primitives.h"
#include "composition.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class VertexBuffer
{
	// The vertex buffer and info
	GLuint buffer;
	size_t buffer_size;

	GLenum type;
	GLuint object_count;
	GLuint elements_per_object;

	// Texture information
	bool uses_texture;
	GLuint texture;

	// Shader information
	GLuint shader;

	GLint aPosition;
	GLint aTexCoord;
	GLint aColor;
	GLint aTile;
	GLint aTileNum;

	GLint uHasTexture;
	GLint uTexture;
	GLint uTransparency;
	GLint uActiveTile;
	
	static GLuint SetUpShader()
	{
		const char* vertexshader =
			"attribute vec3 aPosition;"
			"attribute vec2 aTexCoord;"
			"attribute vec4 aColor;"
			"attribute float aTile;"
			"attribute float aTileNum;"
			
			"varying vec2 vTexCoord;"
			"varying vec4 vColor;"

			"uniform float uActiveTile;"

			"void main()"
			"{"
				// Transforming The Vertex
			"	gl_Position = gl_ModelViewProjectionMatrix * vec4(aPosition, 1.0);"

				// Pass the rest to fragment shader
			"	vTexCoord = aTexCoord;"
			"	vColor = aColor;"
			"	vTexCoord.x += mod(uActiveTile, aTileNum) * aTile;"
			"}"
		;

		const char* fragmentshader =
			"varying vec2 vTexCoord;"
			"varying vec4 vColor;"
			
			"uniform float uHasTexture;"
			"uniform sampler2D uTexture;"
			"uniform float uTransparency;"

			"void main()"
			"{"
			"	vec4 color = (1.0 - uHasTexture) * vColor;"
			"	vec4 texcolor = uHasTexture * texture2D(uTexture, vTexCoord);"
			"	gl_FragColor = color + texcolor;"
			"	gl_FragColor.a *= uTransparency;"
			"}"
		;

		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vshader, 1, &vertexshader, NULL);
		glShaderSource(fshader, 1, &fragmentshader, NULL);
		
		printf("Compiling Vertex Shader...\n");
		glCompileShader(vshader);
		printShaderInfoLog(vshader);

		printf("Compiling Fragment Shader...\n");
		glCompileShader(fshader);
		printShaderInfoLog(fshader);

		GLuint prog = glCreateProgram();
		glAttachShader(prog, fshader);
		glAttachShader(prog, vshader);

		glLinkProgram(prog);

		return prog;
	}

	static void printShaderInfoLog(GLuint obj)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

public:
	VertexBuffer(boost::shared_ptr<Composition> composition)
	{
		buffer_size = composition->BufferSizeInBytes();
		type = composition->PrimitiveType();
		object_count = composition->ObjectCount();
		elements_per_object = composition->ElementsPerObject();
		uses_texture = composition->UsesTexture();

		if (uses_texture)
		{
			SDL_Surface* tex = composition->Texture();

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
		
			glTexImage2D(GL_TEXTURE_2D, 0, 4, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, tex->pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_size, composition->Buffer());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		shader = SetUpShader();

		aPosition = glGetAttribLocation(shader, "aPosition");
		aTexCoord = glGetAttribLocation(shader, "aTexCoord");
		aColor = glGetAttribLocation(shader, "aColor");
		aTile = glGetAttribLocation(shader, "aTile");
		aTileNum = glGetAttribLocation(shader, "aTileNum");
		
		uActiveTile = glGetUniformLocation(shader, "uActiveTile");
		uHasTexture = glGetUniformLocation(shader, "uHasTexture");
		uTexture = glGetUniformLocation(shader, "uTexture");
		uTransparency = glGetUniformLocation(shader, "uTransparency");
	}

	void ChangeQuadAt(size_t index, Quad q)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(Quad), sizeof(Quad), &q);
	}

	void ChangeTriangle(size_t index, Triangle t)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(Triangle), sizeof(Triangle), &t);
	}

	void ChangeElementAt(size_t index, Element e)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(Element), sizeof(Element), &e);
	}

	~VertexBuffer()
	{
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &buffer);
		glDeleteTextures(1, &texture);
		glDeleteProgram(shader);
	}

	void Render(int tile = 0, float transparency = 1.0f)
	{
		glUseProgram(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		
		glEnableVertexAttribArray(aPosition);
		glEnableVertexAttribArray(aTexCoord);
		glEnableVertexAttribArray(aColor);
		glEnableVertexAttribArray(aTile);
		glEnableVertexAttribArray(aTileNum);
		
		glUniform1f(uTransparency, transparency);
		glUniform1f(uHasTexture, uses_texture ? 1.0f : 0.0f);
		glUniform1i(uTexture, 0);
		glUniform1f(uActiveTile, (float)tile);

		glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Element), BUFFER_OFFSET(0));
		glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Element),  BUFFER_OFFSET(sizeof(Vertex)));
		glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Element), BUFFER_OFFSET(sizeof(Vertex) + sizeof(Color)));
		glVertexAttribPointer(aTile, 1, GL_FLOAT, GL_FALSE, sizeof(Element), BUFFER_OFFSET(sizeof(Vertex) + sizeof(Color) + sizeof(TexCoord)));
		glVertexAttribPointer(aTileNum, 1, GL_FLOAT, GL_FALSE, sizeof(Element), BUFFER_OFFSET(sizeof(Vertex) + sizeof(Color) + sizeof(TexCoord) + sizeof(float)));
		
		glDrawArrays(type, 0, object_count * elements_per_object);

		glDisableVertexAttribArray(aPosition);
		glDisableVertexAttribArray(aTexCoord);
		glDisableVertexAttribArray(aColor);
		glDisableVertexAttribArray(aTile);
		glEnableVertexAttribArray(aTileNum);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

#endif // VERTEXBUFFER_H
