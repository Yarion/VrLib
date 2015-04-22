#pragma once

#include <gl/glew.h>
#include <gl/GL.h>
#include <glm/glm.hpp>

namespace vrlib
{
	namespace gl
	{

		class Vertex
		{
		public:
		};


		struct VertexP3
		{
			float px, py, pz;
		};

		struct VertexP3N3
		{
			float px, py, pz;
			float nx, ny, nz;
		};

		struct VertexP3N3T2
		{
			float px, py, pz;
			float nx, ny, nz;
			float tx, ty;
		};


		template<class T>	inline void setP3(T& vertex, const glm::vec3 &p)								{	}
		template<>			inline void setP3<VertexP3>(VertexP3& vertex, const glm::vec3 &p)				{ vertex.px = p.x;		vertex.py = p.y;	vertex.pz = p.z; }
		template<>			inline void setP3<VertexP3N3>(VertexP3N3& vertex, const glm::vec3 &p)			{ vertex.px = p.x;		vertex.py = p.y;	vertex.pz = p.z; }
		template<>			inline void setP3<VertexP3N3T2>(VertexP3N3T2& vertex, const glm::vec3 &p)		{ vertex.px = p.x;		vertex.py = p.y;	vertex.pz = p.z; }

		template<class T>	inline void setN3(T& vertex, const glm::vec3 &n)								{	}
		template<>			inline void setN3<VertexP3N3>(VertexP3N3& vertex, const glm::vec3 &n)			{ vertex.nx = n.x;		vertex.ny = n.y;	vertex.nz = n.z; }
		template<>			inline void setN3<VertexP3N3T2>(VertexP3N3T2& vertex, const glm::vec3 &n)		{ vertex.nx = n.x;		vertex.ny = n.y;	vertex.nz = n.z; }

		template<class T>	inline void setT2(T& vertex, const glm::vec2 &t)								{	}
		template<>			inline void setT2(VertexP3N3T2& vertex, const glm::vec2 &t)						{ vertex.tx = t.x;		vertex.ty = t.y;  }


		//// WATCH OUT, NEEDS TO BE SPECIALIZED IN THE RIGHT ORDER
		template<class T>	inline int setAttribute(int& attributeIndex, int totalSize, int offset)	{}
		template<>			inline int setAttribute<VertexP3>(int& attributeIndex, int totalSize, int offset)
		{
			int prevSize = 0;
			glEnableVertexAttribArray(attributeIndex);
			glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, totalSize, (void*)(offset + prevSize));
			attributeIndex++;
			return prevSize + 3 * sizeof(float);
		}
		template<>			inline int setAttribute<VertexP3N3>(int& attributeIndex, int totalSize, int offset)
		{
			int prevSize = setAttribute<VertexP3>(attributeIndex, totalSize, offset);
			glEnableVertexAttribArray(attributeIndex);
			glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, totalSize, (void*)(offset + prevSize));
			attributeIndex++;
			return prevSize + 3 * sizeof(float);
		}

		template<>			inline int setAttribute<VertexP3N3T2>(int& attributeIndex, int totalSize, int offset)
		{
			int prevSize = setAttribute<VertexP3N3>(attributeIndex, totalSize, offset);
			glEnableVertexAttribArray(attributeIndex);
			glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, totalSize, (void*)(offset + prevSize));
			attributeIndex++;
			return prevSize + 2 * sizeof(float);
		}







		template<class T>	inline void setAttributes(int offset)											
		{
			int i = 0;
			setAttribute<T>(i, sizeof(T), offset);
		}




		


		class VertexPosition : public Vertex
		{
		public:
			glm::vec3 position;

			VertexPosition(glm::vec3 position)
			{
				this->position = position;
			}

			static int getSize()
			{
				return sizeof(GL_FLOAT) * 3;
			}
			static void setPointer(int totalSize)
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, totalSize, 0);
			}
			static void unsetPointer()
			{
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			static int setVAO(int totalSize)
			{
				int index = 0;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, totalSize, (void*)0);
				return index;
			}
		};

		class VertexPositionColor : public VertexPosition
		{
		public:
			glm::vec4 color;


			VertexPositionColor(const glm::vec3 &position, const glm::vec4 &color) : VertexPosition(position)
			{
				this->color = color;
			}

			static int getSize()
			{
				return VertexPosition::getSize() + sizeof(GL_FLOAT) * 4;
			}
			static void setPointer(int totalSize)
			{
				VertexPosition::setPointer(totalSize);
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_FLOAT, totalSize, (void*)VertexPosition::getSize());
			}
			static void unsetPointer()
			{
				VertexPosition::unsetPointer();
				glDisableClientState(GL_COLOR_ARRAY);
			}
			static int setVAO(int totalSize)
			{
				int index = VertexPosition::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPosition::getSize());
				return index;
			}
		};

		class VertexPositionTexture3 : public VertexPosition
		{
		public:
			glm::vec3 texCoord;


			VertexPositionTexture3(glm::vec3 position, glm::vec3 texCoord) : VertexPosition(position)
			{
				this->texCoord = texCoord;
			}

			static int getSize()
			{
				return VertexPosition::getSize() + sizeof(GL_FLOAT) * 3;
			}
			static void setPointer(int totalSize)
			{
				VertexPosition::setPointer(totalSize);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(3, GL_FLOAT, totalSize, (void*)VertexPosition::getSize());
			}
			static void unsetPointer()
			{
				VertexPosition::unsetPointer();
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			static int setVAO(int totalSize)
			{
				int index = VertexPosition::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPosition::getSize());
				return index;
			}
		};


		class VertexPositionTexture : public VertexPosition
		{
		public:
			glm::vec2 texCoord;

			VertexPositionTexture(glm::vec3 position, glm::vec2 texCoord) : VertexPosition(position)
			{
				this->texCoord = texCoord;
			}

			static int getSize()
			{
				return VertexPosition::getSize() + sizeof(GL_FLOAT) * 2;
			}
			static void setPointer(int totalSize)
			{
				VertexPosition::setPointer(totalSize);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, totalSize, (void*)VertexPosition::getSize());
			}
			static void unsetPointer()
			{
				VertexPosition::unsetPointer();
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			static int setVAO(int totalSize)
			{
				int index = VertexPosition::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPosition::getSize());
				return index;
			}
		};

		class VertexPositionTextureNormal : public VertexPositionTexture
		{
		public:
			glm::vec3 normal;


			VertexPositionTextureNormal(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal) : VertexPositionTexture(position, texCoord)
			{
				this->normal = normal;
			}
			static int getSize()
			{
				return VertexPositionTexture::getSize() + sizeof(GL_FLOAT) * 3;
			}
			static void setPointer(int totalSize)
			{
				VertexPositionTexture::setPointer(totalSize);
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, totalSize, (void*)VertexPositionTexture::getSize());
			}
			static void unsetPointer()
			{
				VertexPositionTexture::unsetPointer();
				glDisableClientState(GL_NORMAL_ARRAY);
			}
			static int setVAO(int totalSize)
			{
				int index = VertexPositionTexture::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPositionTexture::getSize());
				return index;
			}
		};


		class VertexPositionColorNormal : public VertexPositionColor
		{
		public:
			glm::vec3 normal;


			VertexPositionColorNormal(const glm::vec3 &position, const glm::vec4 &color, const glm::vec3 &normal) : VertexPositionColor(position, color)
			{
				this->normal = normal;
			}
			static int getSize()
			{
				return VertexPositionColor::getSize() + sizeof(GL_FLOAT) * 3;
			}
			static void setPointer(int totalSize)
			{
				VertexPositionColor::setPointer(totalSize);
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, totalSize, (void*)VertexPositionColor::getSize());
			}
			static void unsetPointer()
			{
				VertexPositionColor::unsetPointer();
				glDisableClientState(GL_NORMAL_ARRAY);
			}
			static int setVAO(int totalSize)
			{
				int index = VertexPositionColor::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPositionColor::getSize());
				return index;
			}
		};

		class VertexPositionNormal : public VertexPosition
		{
		public:
			glm::vec3 normal;


			VertexPositionNormal(const glm::vec3 &position, const glm::vec3 &normal) : VertexPosition(position)
			{
				this->normal = normal;
			}
			static int getSize()
			{
				return VertexPosition::getSize() + sizeof(GL_FLOAT) * 3;
			}
			static void setPointer(int totalSize)
			{
				VertexPosition::setPointer(totalSize);
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, totalSize, (void*)VertexPosition::getSize());
			}
			static void unsetPointer()
			{
				VertexPosition::unsetPointer();
				glDisableClientState(GL_NORMAL_ARRAY);
			}
			static int setVAO(int totalSize)
			{
				int index = VertexPosition::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPosition::getSize());
				return index;
			}
		};


		class VertexPositionColorTexture : public VertexPositionColor
		{
		public:
			glm::vec2 texCoord;

			VertexPositionColorTexture(const glm::vec3 &position, const glm::vec4 &color, const glm::vec2 &texCoord) : VertexPositionColor(position, color)
			{
				this->texCoord = texCoord;
			}
			static int getSize()
			{
				return VertexPositionColor::getSize() + sizeof(GL_FLOAT) * 2;
			}
			static void setPointer(int totalSize)
			{
				VertexPosition::setPointer(totalSize);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, totalSize, (void*)VertexPositionColor::getSize());
			}
			static void unsetPointer()
			{
				VertexPositionColor::unsetPointer();
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			static int setVAO(int totalSize)
			{
				int index = VertexPositionColor::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPositionColor::getSize());
				return index;
			}
		};

		class VertexPositionColorNormalTexture : public VertexPositionColorNormal
		{
		public:
			glm::vec2 texCoord;

			VertexPositionColorNormalTexture(const glm::vec3 &position, const glm::vec4 &color, const glm::vec3 &normal, const glm::vec2 &texCoord) : VertexPositionColorNormal(position, color, normal)
			{
				this->texCoord = texCoord;
			}

			static int getSize()
			{
				return VertexPositionColorNormal::getSize() + sizeof(GL_FLOAT) * 2;
			}
			static void setPointer(int totalSize)
			{
				VertexPositionColorNormal::setPointer(totalSize);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, totalSize, (void*)VertexPositionColorNormal::getSize());
			}
			static void unsetPointer()
			{
				VertexPositionColorNormal::unsetPointer();
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

			static int setVAO(int totalSize)
			{
				int index = VertexPositionColorNormal::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPositionColorNormal::getSize());
				return index;
			}

		};


		class VertexPositionNormalTexture : public VertexPositionNormal
		{
		public:
			glm::vec2 texCoord;

			VertexPositionNormalTexture(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord) : VertexPositionNormal(position, normal)
			{
				this->texCoord = texCoord;
			}

			static int getSize()
			{
				return VertexPositionNormal::getSize() + sizeof(GL_FLOAT) * 2;
			}
			static void setPointer(int totalSize)
			{
				VertexPositionNormal::setPointer(totalSize);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, totalSize, (void*)VertexPositionNormal::getSize());
			}
			static void unsetPointer()
			{
				VertexPositionNormal::unsetPointer();
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

			static int setVAO(int totalSize)
			{
				int index = VertexPositionNormal::setVAO(totalSize) + 1;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, totalSize, (void*)VertexPositionNormal::getSize());
				return index;
			}

		};
	}
}
