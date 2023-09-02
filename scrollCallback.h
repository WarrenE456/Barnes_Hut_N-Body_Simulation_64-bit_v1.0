#pragma once

extern float zoom;

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
	{
		zoom = zoom * 1.025;
	}
	else
	{
		zoom = zoom / 1.025;
	}
	return;
}