#include "Frustum.h"

#include <glm/gtc/matrix_access.hpp>


namespace vrlib
{
	namespace math
	{
		Frustum::Frustum(const glm::mat4 & projectionMatrix, const glm::mat4 & modelviewMatrix)
		{
			setFromMatrix(projectionMatrix, modelviewMatrix);
		}

		void Frustum::setFromMatrix(const glm::mat4 &projectionMatrix, const glm::mat4 &modelviewMatrix)
		{ // http://www.lighthouse3d.com/tutorials/view-frustum-culling/clip-space-approach-extracting-the-planes/
			// Extracting the planes.
			this->projectionMatrix = projectionMatrix;
			this->modelviewMatrix = modelviewMatrix;
			glm::mat4 matrix = projectionMatrix * modelviewMatrix;

			glm::vec4 rowX = glm::row(matrix, 0);
			glm::vec4 rowY = glm::row(matrix, 1);
			glm::vec4 rowZ = glm::row(matrix, 2);
			glm::vec4 rowW = glm::row(matrix, 3);

			planes[0] = glm::normalize(rowW + rowX);
			planes[1] = glm::normalize(rowW - rowX);
			planes[2] = glm::normalize(rowW + rowY);
			planes[3] = glm::normalize(rowW - rowY);
			planes[4] = glm::normalize(rowW + rowZ);
			planes[5] = glm::normalize(rowW - rowZ);


			// Normalizing the planes.
			for (int i = 0; i < 4; i++)
				planes[i] = -planes[i] / glm::length(glm::vec3(planes[i]));
		}
		glm::vec3 Frustum::getCenter()
		{
			glm::mat4 mat = glm::inverse(projectionMatrix);
			auto drawVert = [&, this](const glm::vec3 &pos)
			{
				glm::vec4 p(mat * glm::vec4(pos, 1));
				p = glm::vec4(p.x * p.w, p.y * p.w, p.z * p.w, 1);
				p = glm::inverse(modelviewMatrix) * p;
				return glm::vec3(p);
			};
			
			glm::vec3 total(0, 0, 0);
			for (int i = 0; i < 8; i++)
				total += drawVert(glm::vec3(((i >> 0) & 1) * 2 - 1, ((i >> 1) & 1) * 2 - 1, ((i >> 2) & 1)));
			return total /= 8.0f;
		}
	}
}