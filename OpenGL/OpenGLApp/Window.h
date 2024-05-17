#pragma once


class GLFWwindow;


class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

public:
	GLfloat GetBufferWidth() const { return bufferSize.first; }
	GLfloat GetBufferHeight() const { return bufferSize.second; }

	const std::array<bool, 1024>& GetKeys() const { return keys; }
	bool GetShouldClose() const;

	GLfloat EmitXChanges();
	GLfloat EmitYChanges();


public:
	int Initialize();
	void SwapBuffers();

private:
	static void _HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void _HandleMouse(GLFWwindow* window, double xPos, double yPos);

private:
	void _InitCallbacks();


private:
	GLFWwindow* mainWindow{};

	std::pair<GLint, GLint> windowSize{};
	std::pair<GLint, GLint> bufferSize{};

	std::array<bool, 1024> keys{};

	GLfloat lastX{};
	GLfloat lastY{};
	GLfloat xChange{};
	GLfloat yChange{};
	bool mouseFirstMove;
};

