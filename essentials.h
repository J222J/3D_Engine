#pragma once

#include<cmath>
#include<fstream>
#include<functional>
#include<GL/freeglut.h>
#include<iostream>
#include<string>
#include<vector>

class position2D;
class colour;

void line(position2D a, position2D b, colour color, float thickness);

class vector2D {
public:
	float x, y;

	float length() {
		return sqrt(x*x + y*y);
	}

	vector2D normalized() {
		float len = length();
		vector2D normalized_vector = vector2D{x/len, y/len};

		return normalized_vector;
	}

	vector2D(float xx, float yy) {
		x = xx;
		y = yy;
	}

	~vector2D() = default;
};

class vector3D {
public:
	float x, y, z;

	float length() {
		return sqrt(x*x + y*y + z*z);
	}

	vector3D normalized() {
		float len = length();
		vector3D normalized_vector = vector3D{x/len, y/len, z/len};

		return normalized_vector;
	}

	vector3D(float xx, float yy, float zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	~vector3D() = default;
};

class position2D {
public:
	float x, y;

	position2D(float xx, float yy) {
		x = xx;
		y = yy;
	}

	~position2D() = default;
};

class position3D {
public:
	float x, y, z;

	position3D(float xx, float yy, float zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	~position3D() = default;
};

class colour {
public:
	float r, g, b; // value between 0.0f -> 1.0f

	colour(int rr, int gg, int bb) {
		r = 1.0f * (rr / 255.0f);
		g = 1.0f * (gg / 255.0f);
		b = 1.0f * (bb / 255.0f);
	}

	~colour() = default;
};

class object { // basically .obj file
private:
	std::vector<std::string> separate(std::string line) {
		std::vector<std::string> strings;
		int ind = 2;
		std::string buffer = "";
		while (ind < line.size()) {
			if (line[ind] == ' ') {
				if (!buffer.empty())
					strings.push_back(buffer);
				buffer = "";
			} else {
				buffer += line[ind];
			}
			ind++;
		}
		strings.push_back(buffer);

		return strings;
	}

	std::vector<int> separate_into_vector(std::string line) {
		std::vector<int> ans;

		std::vector<std::string> strs = separate(line);
		std::string buff;
		for (auto e : strs) {
			buff = "";
			for (auto c : e)
				if (c != '/') {
					buff += c;
				} else {
					break;
				}

			ans.push_back(std::stof(buff));
		}

		return ans;
	}

	position3D separate_into_position3D(std::string line) {
		std::vector<std::string> strs = separate(line);

		return position3D{std::stof(strs[0]), std::stof(strs[1]), std::stof(strs[2])};
	}

public:
	std::vector<position3D> vertices;
	std::vector<std::vector<int>> faces;

	object(std::string path) {
		std::ifstream obj(path);
		if (!obj.is_open()) {
			std::cerr<< "file : " << path << " doesn't exist.\n";
			std::cout<< "file : " << path << " doesn't exist.\n";
			return;
		}

		std::string line;
		while (std::getline(obj, line)) {
			if (line[0] == 'v' and line[1] == ' ') {// add to vertices
				vertices.push_back(separate_into_position3D(line));
			} else if (line[0] == 'f' and line[1] == ' ') {
				faces.push_back(separate_into_vector(line));
			}
		}

		std::cout<< "POST COMPUTATION CHECKS :\n";
		std::cout<< "vertices : " << vertices.size() << '\n';
		for (auto e : vertices) {
			std::cout<< "\t(" << e.x << "; " << e.y << "; " << e.z << ")\n";
		}
		std::cout<< "faces : " << faces.size() << '\n';
		for (auto e : faces) {
			std::cout<< '\t';
			for (int ind : e)
				std::cout<< ind << ' ';
			std::cout<< '\n';
		}
	}

	~object() = default;
};

class camera {
public:
	float rotation_x, rotation_y, rotation_z;
	float x, y, z; // center of the screen
	float focal_length; // position of the focal point when no rotation is applied is x, y, z-focal_length
	float width, height; // best for this to be the same as the screen resolution

	position2D on_screen(position3D position) { // find where a position would be drawn on screen
		float yp = focal_length*(position.y-y)/(position.z-z+focal_length);
		float coordinate_y = yp + height/2;

		float xp = focal_length*(position.x-x)/(position.z-z+focal_length);
		float coordinate_x = xp + width/2;

		return position2D{coordinate_x, coordinate_y};
	}

	position3D return_position3D() {
		return position3D{x, y, z};
	}

	void draw_object(object obj) {
		std::vector<position3D> verts = obj.vertices;

		for (auto face : obj.faces) {
			for (int i = 0; i < face.size(); i++) {
				line(on_screen(verts[face[i]-1]), on_screen(verts[face[(i+1)%face.size()]-1]), colour{0, 0, 0}, 1.0f);
			}
		}
	}

	camera(float xx, float yy, float zz, float ifocal_length, float iwidth, float iheight) {
		x = xx;
		y = yy;
		z = zz;

		focal_length = ifocal_length;
		width = iwidth;
		height = iheight;
	}

	~camera() = default;
};

void line(position2D a, position2D b, colour color, float thickness) { // I'll maybe handle this later, std::function<bool(position2D)> onscreen = [&]{return true;}) {
	std::cout<< "FROM : (" << a.x << "; " << a.y << "), TO : (" << b.x << "; " << b.y << ")\n";

	float dx = b.x-a.x;
	float dy = b.y-a.y;

	int steps = std::max(std::abs(dx), std::abs(dy));
	float x_step = (float)dx/steps;
	float y_step = (float)dy/steps;

	float cur_x = a.x;
	float cur_y = a.y;

	glColor3f(color.r, color.g, color.b);
	glPointSize(thickness);

	steps++;
	glBegin(GL_POINTS);
	while (steps--) {
		glVertex2f(cur_x, cur_y);
		cur_x += x_step;
		cur_y += y_step;
	}
	glEnd();
}