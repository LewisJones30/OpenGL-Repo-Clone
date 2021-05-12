#pragma once

void init(void);

void makeObject(int VAOnumber);
void modelBind(float x, float y, float z);
void loadTexture(GLuint& texture, std::string texturepath);
void moveCameraInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void RandomMusicChange();
void ForcedTrackChange(int trackNumber);
void ControlAmbientLighting(float lightIntensity);