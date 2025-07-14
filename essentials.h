#pragma once

#include<cmath>
#include<fstream>
#include<functional>
#include<GL/freeglut.h>
#include<iostream>
#include<string>
#include<vector>

const float PI = 3.1415926536;

class position2D;
class colour;

void clamp(float &value, float left, float right) {
	if (value < left)
		value = left;
	else if (value > right)
		value = right;
}

bool default_ondisplay(position2D pos);

void line(position2D a, position2D b, colour color, float thickness, bool (*on_display)(position2D) = &default_ondisplay);

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

private:
	void rotate_x(float angle) {
		float new_y = y * cos(angle) - z * sin(angle);
		float new_z = y * sin(angle) + z * cos(angle);

		y = new_y;
		z = new_z;
	}

	void rotate_y(float angle) {
		float new_x = x * cos(angle) - z * sin(angle);
		float new_z = x * sin(angle) + z * cos(angle);

		x = new_x;
		z = new_z;
	}

	void rotate_z(float angle) {
		float new_y = y * cos(angle) - x * sin(angle);
		float new_x = y * sin(angle) + x * cos(angle);

		y = new_y;
		x = new_x;
	}

public:

	float length() {
		return sqrt(x*x + y*y + z*z);
	}

	void rotate(float x_rotation, float y_rotation, float z_rotation) { // x_rotation -> around x axis, y_rotation -> y axis, z_rotation -> z axis
		rotate_y(y_rotation * PI / 180);
		rotate_x(x_rotation * PI / 180);
		rotate_z(z_rotation * PI / 180);
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

float dot_product(vector3D a, vector3D b) { // I know this as scalar product so this comment is just for me
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

float angle(vector3D a, vector3D b) {
	float dot_p = dot_product(a, b);
	dot_p /= a.length() * b.length();

	std::cout<< dot_p << '\n';
	return acos(dot_p) * 180 / PI;
}

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

private:
	void rotate_x(float angle) {
		float new_y = y * cos(angle) - z * sin(angle);
		float new_z = y * sin(angle) + z * cos(angle);

		y = new_y;
		z = new_z;
	}

	void rotate_y(float angle) {
		float new_x = x * cos(angle) - z * sin(angle);
		float new_z = x * sin(angle) + z * cos(angle);

		x = new_x;
		z = new_z;
	}

	void rotate_z(float angle) {
		float new_y = y * cos(angle) - x * sin(angle);
		float new_x = y * sin(angle) + x * cos(angle);

		y = new_y;
		x = new_x;
	}

public:
	
	void rotate(float x_rotation, float y_rotation, float z_rotation, position3D around) { // x_rotation -> around x axis, y_rotation -> y axis, z_rotation -> z axis
		x -= around.x;
		y -= around.y;
		z -= around.z;

		rotate_y(y_rotation * PI / 180);
		rotate_x(x_rotation * PI / 180);
		rotate_z(z_rotation * PI / 180);
	
		x += around.x;
		y += around.y;
		z += around.z;
	}

	position3D rotated(float x_rotation, float y_rotation, float z_rotation, position3D around) {
		position3D copy(x, y, z);

		copy.rotate(x_rotation, y_rotation, z_rotation, around);

		return copy;
	}

	position3D(float xx, float yy, float zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	~position3D() = default;
};

vector3D to_vector(position3D a, position3D b) { // vector from position A to position B dunno wtf else to name this shit
	return vector3D{b.x-a.x, b.y-a.x, b.z-a.z};
}

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
public:
	float rotation_x, rotation_y, rotation_z;
	float x, y, z;
	std::vector<position3D> vertices;
	std::vector<std::vector<int>> faces;

private:
/*	void rotate_x(float angle) {
		float new_y = y * cos(angle) - z * sin(angle);
		float new_z = y * sin(angle) + z * cos(angle);

		y = new_y;
		z = new_z;

		for (auto &e : vertices) {
			new_y = e.y * cos(angle) - e.z * sin(angle);
			new_z = e.y * sin(angle) + e.z * cos(angle);

			e.y = new_y;
			e.z = new_z;
		}
	}

	void rotate_y(float angle) {
		float new_x = x * cos(angle) - z * sin(angle);
		float new_z = x * sin(angle) + z * cos(angle);

		x = new_x;
		z = new_z;

		for (auto &e : vertices) {
			new_x = e.x * cos(angle) - e.z * sin(angle);
			new_z = e.x * sin(angle) + e.z * cos(angle);

			e.x = new_x;
			e.z = new_z;
		}
	}

	void rotate_z(float angle) {
		float new_y = y * cos(angle) - x * sin(angle);
		float new_x = y * sin(angle) + x * cos(angle);

		y = new_y;
		x = new_x;

		for (auto &e : vertices) {
			new_y = e.y * cos(angle) - e.x * sin(angle);
			new_x = e.y * sin(angle) + e.x * cos(angle);

			e.y = new_y;
			e.x = new_x;
		}
	}
*/

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

		return position3D{std::stof(strs[0]), -std::stof(strs[1]), std::stof(strs[2])};
	}

public:

	/*void rotate(float x_rotation, float y_rotation, float z_rotation, position3D around) { // x_rotation -> around x axis, y_rotation -> y axis, z_rotation -> z axis
		x -= around.x;
		y -= around.y;
		z -= around.z;
		for (auto &e : vertices) {
			e.x -= around.x;
			e.y -= around.y;
			e.z -= around.z;
		}

		rotate_y(y_rotation * PI / 180);
		rotate_x(x_rotation * PI / 180);
		rotate_z(z_rotation * PI / 180);
	
		x += around.x;
		y += around.y;
		z += around.z;
		for (auto &e : vertices) {
			e.x += around.x;
			e.y += around.y;
			e.z += around.z;
		}
	}*/

	position3D return_position3D() {
		return position3D{x, y, z};
	}

	object(std::string path, position3D position = position3D{0, 0, 0}) {
		x = position.x;
		y = position.y;
		z = position.z;

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

		/*std::cout<< "POST COMPUTATION CHECKS :\n";
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
		}*/
	}

	~object() = default;
};

class camera {
public:
	float rotation_x, rotation_y, rotation_z; // in degrees
	float x, y, z; // center of the screen || changing this to position of focal point
		float focal_length; // position of the focal point when no rotation is applied is {x, y, z-focal_length} || will be changed to {x, y, z}
	float width, height; // (resolution) best for this to be the same as the screen resolution, if not a custom handler for displaying what the camera sees will need to be coded.

	float cam_width = 1.0f, cam_height; // actual width/height

	position3D return_position3D() {
		return position3D{x, y, z}; // this should be {x, y, z-focal_length} but rn it gets fucked if I do that || being changed
	}

	vector3D forward() {
		vector3D vec(0, 0, 1);
		vec.rotate(rotation_x, rotation_y, rotation_z);

		return vec;
	}

	/* this entire shit might need to be re-written || currently re-writing this part
	position2D on_screen(position3D position) { // find where a position would be drawn on screen
		vector3D forw = forward();
		vector3D to_pos = to_vector(return_position3D(), position);

		std::cout<< dot_product(to_pos, forw) << '\n';
		if (dot_product(forw, to_pos) < 0)
			return position2D{-1, -1};

		position.rotate(-rotation_x, -rotation_y, -rotation_z, return_position3D());

		float x1 = position.x;
		float y1 = position.y;
		float z1 = position.z;

		float yp = focal_length*200*(y1-y)/(200*z1-200*z+focal_length);
		float coordinate_y = yp + height/2;

		float xp = focal_length*200*(x1-x)/(200*z1-200*z+focal_length);
		float coordinate_x = xp + width/2;

		return position2D{coordinate_x, coordinate_y};
	} */

	position2D on_screen(position3D position, position2D offset) {
		// add a check here to see if it's off screen maybe?
		vector3D forw = forward();
		vector3D to_pos = to_vector(return_position3D(), position);

		if (dot_product(forw, to_pos) < 0)
			return position2D{-1, -1};

		position.rotate(-rotation_x, -rotation_y, -rotation_z, return_position3D());

		float x1 = position.x;
		float y1 = position.y;
		float z1 = position.z;

		float yp = focal_length * (y1-y) / (z1-z);
		float relative_coordinate_y = yp + cam_height/2;

		float xp = focal_length * (x1-x) / (z1-z);
		float relative_coordinate_x = xp + cam_width/2;

		float coordinate_y = height * relative_coordinate_y / cam_height;
		float coordinate_x = width * relative_coordinate_x / cam_width;

		return position2D{coordinate_x+offset.x, coordinate_y+offset.y};
	}

	void move(vector3D direction, float mult) {
		x += direction.x * mult;
		y += direction.y * mult;
		z += direction.z * mult;
	}

	void draw_object(object obj, position2D offset = position2D{-1, -1}, bool (*on_display)(position2D) = &default_ondisplay) {
		std::vector<position3D> verts = obj.vertices;

		for (auto face : obj.faces) {
			for (int i = 0; i < face.size(); i++) {
				line(on_screen(verts[face[i]-1].rotated(obj.rotation_x, obj.rotation_y, obj.rotation_z, obj.return_position3D()), offset), on_screen(verts[face[(i+1)%face.size()]-1].rotated(obj.rotation_x, obj.rotation_y, obj.rotation_z, obj.return_position3D()), offset), colour{0, 0, 0}, 1.0f, on_display);
			}
		}
	}

	camera(float xx, float yy, float zz, float ifocal_length, float iwidth, float iheight) {
		x = xx;
		y = yy;
		z = zz;

		focal_length = ifocal_length*10;
		width = iwidth;
		height = iheight;

		cam_height = height/width;
	}

	~camera() = default;
};

bool operator==(const position2D& a, const position2D& b) {
	return (a.x == b.x and a.y == b.y);
}

bool operator==(const position3D& a, const position3D& b) {
	return (a.x == b.x and a.y == b.y and a.z == b.z);
}

bool operator==(const vector2D& a, const vector2D& b) {
	return (a.x == b.x and a.y == b.y);
}

bool operator==(const vector3D& a, const vector3D& b) {
	return (a.x == b.x and a.y == b.y and a.z == b.z);
}

bool operator==(const colour& a, const colour& b) {
	return (a.r == b.r and a.g == b.g and a.b == b.b);
}

bool default_ondisplay(position2D pos) {
	return true;
}

void line(position2D a, position2D b, colour color, float thickness, bool (*on_display)(position2D)) { //= &default_ondisplay) {
	//std::cout<< "FROM : (" << a.x << "; " << a.y << "), TO : (" << b.x << "; " << b.y << ")\n";
	if (!on_display(a) and !on_display(b))
		return;
	if (a == position2D{-1, -1} or b == position2D{-1, -1})
		return;

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
		if (on_display(position2D{cur_x, cur_y}))
			glVertex2f(cur_x, cur_y);
		cur_x += x_step;
		cur_y += y_step;
	}
	glEnd();
}