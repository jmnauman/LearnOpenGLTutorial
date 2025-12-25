#include "vaoLibrary.h"


// Usually when you have multiple objects, you first generatte/configure all the VAOs (attribute pointers + VBOs) then store for later use.
// When using a VBO like this, we call glDrawArrays
GLuint getTriangleVAO()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// This is a general pattern in OpenGL. Has a concept of Objects. Object is a collection of options that repesent a subset of OpenGL's state.
	// Can visualize them as a C-like struct.
	// Ask OpenGL to create the object and give you an ID you can use to access it
	// Bind the object to part of the GL context's state.
	// Set options on the object
	// I'm thinking these objects remain allocated under the hood. Unbinding them just removes them from context state. Looks like they may have to be destroyed explicitly to be relasesd
	GLuint VBO; // vertex buffer object, used to manage which vertices we've sent to the GPU
	glGenBuffers(1, &VBO); // first argument is the number of buffers to generate, second argument is an array of ids. Buffers are instatiated behind the scenes

	// VAO - Vertex Array Object. Benefit is that when configuring attribute pointers, we can store that info in the VAO. Then just
// rebind the VAO when we need to draw the object again, instead of repeating the above every time for different objects. Makes it easier to switch
// between different vertex data and attribute configurations.
// OpenGL Core REQUIRES a VAO
// only needs to be done once
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Can bind several buffers at once if they have separate types. This is the buffer type of a vertex buffer

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Allocates memory and stores data in the buffer bound to the target

	/* usage arg tells the graphics card how we want it to manage the given data. Sounds like it may determine what kind of memory the buffer goes into
	 *GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
GL_STATIC_DRAW: the data is set only once and used many times.
GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
*/
// We need to specify which part of input data goes to which vertex attribute.
// Remember that in the shader, we spcifed location of position attribute at 0.
// So this essentially says "the shader position input has a size of 3 (3 values), the positions are floats, they aren't normalized between 0 and 1,
// each position is 12 bytes apart in the input data. (Since the values are tightly packed, we technically could pass 0 for the stride and openGL would figure it out)
// Each vertex attribute takes memory from a VBO. The VBO used is what we've currently bound to GL_ARRAY_BUFFER
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// This enabled the vertex attribute we just described to OpenGL in the previous line. Attributes are disabled by default.
	glEnableVertexAttribArray(0);

	// This is safe to do. glVertexAttribPoiunter registered the VBO as the bound buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// We typically don't call this, because modifying other VAOs required binding to them.
	// NOTE: Worth remembering that I hadn't bound the VAO in here before, which led to a bug where the triangle would be drawn.
	// I'm guessing this is because OpenGL didn't link the buffer or vertex attribute pointer to the VAO, since it wasn't "active".
	glBindVertexArray(0);

	return VAO;
}

// WHen we draw with an element buffer,we call glDrawElements
GLuint getRectangleVAO(float texScale, float offset)
{
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   texScale + offset, texScale + offset,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   texScale + offset, offset,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   offset, offset,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   offset, texScale + offset    // top left 
	};

	unsigned int indices[]
	{
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Think of EBO as an index buffer, so we don't have redundant vertices in this example of a rectangle
	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// VERY IMPORTANT. YOU NEED TO UNBIND VAO FIRST. The vao records the bind buffer calls. Which means if we unbind the others first, they end up unbound in the VAO
	// Last element buffer bound while the VAO is bound will be rebound when the VAO is rebound
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}

// Exercise called for VAO, so that's why we're doing it.
GLuint getTwoTrianglesVAO()
{
	float vertices[] = {
		-0.5, -0.5, 0, // bot left
		-0.5, 0.5, 0, // up left
		0.5, 0.5, 0, // up right
		-0.5, -0.5, 0, // bot left
		0.5, 0.5, 0, // up right
		0.5, -0.5, 0 //bot right 
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	// VERY IMPORTANT. YOU NEED TO UNBIND VAO FIRST. The vao records the bind buffer calls. Which means if we unbind the others first, they end up unbound in the VAO
	// Last element buffer bound while the VAO is bound will be rebound when the VAO is rebound
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return VAO;
}

GLuint getTriangleTwoVAO()
{
	float vertices[] = {
		-0.5, -0.5, 0,
		0.5, 0.5, 0,
		0.5, -0.5, 0
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glEnableVertexAttribArray(0);

	return VAO;
}

// Without using a texture, this produces a rainbow effect because the output color from the vertex is interpolated between the verts based on
// fragment position.
GLuint getTriangleVAOWithTexCoord()
{
	// First 3 of each row are positions, second 3 of each row are colors, third two are tex coords
	// The stride is now 32 for everything, 12 for each attribute.
	float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 0.f, 0.f,
	 0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 1.f, 0.f,
	 0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f, 0.5f, 1.f
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	// Size appears to be the number of components. The stride is the stride for an ENTIRE VERTEX.
	// The last argument gives the offset of the attribute within a single vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// tex coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

constexpr float boxVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
};

GLuint getBoxVAO()
{
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vao;
}

// note that it would be more efficient to have the box and the light source share the same VBO.
GLuint getLightSourceVAO()
{
	GLuint lightVAO = 0;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	GLuint lightVBO = 0;
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return lightVAO;
}

std::vector<glm::vec3> tenRandomPositions()
{
	return std::vector<glm::vec3> {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
}
