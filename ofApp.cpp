#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();
	
	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	ofColor color;
	vector<int> hex_list = { 0x247BA0, 0x70C1B3, 0xB2DBBF, 0xF3FFBD, 0xFF1654 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	int len = 300;
	float width = 6.5;
	int param_len = 145;
	float z_span = 3;

	if (ofRandom(100) < 5) {
		
		this->z_list.push_back(len * -2);
		this->param_list.push_back(ofRandom(100, 200));
		this->color_list.push_back(this->base_color_list[ofRandom(this->base_color_list.size())]);
	}

	for (int i = this->z_list.size() -1; i > -1; i--) {

		float z = this->z_list[i];
		bool has_vertex = false;
		int start_index = this->frame.getNumVertices();
		for (int param = this->param_list[i] + param_len; param > this->param_list[i]; param--) {

			auto location = glm::vec3(this->make_point(len, param), z);
			if (location.z >= len * -2 && location.z < len * 2) {

				vector<glm::vec3> vertices;

				vertices.push_back(location + glm::vec3(0, 0, -width));
				vertices.push_back(location + glm::vec3(0, 0, width));

				this->face.addVertices(vertices);
				this->frame.addVertices(vertices);

				for (int k = 0; k < 2; k++) {

					this->face.addColor(this->color_list[i]);
				}

				if (has_vertex) {

					this->face.addIndex(this->face.getNumVertices() - 4); this->face.addIndex(this->face.getNumVertices() - 3); this->face.addIndex(this->face.getNumVertices() - 2);
					this->face.addIndex(this->face.getNumVertices() - 3); this->face.addIndex(this->face.getNumVertices() - 2); this->face.addIndex(this->face.getNumVertices() - 1);

					this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 2);
					this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 1);
				}

				has_vertex = true;
			}

			z -= z_span;
		}

		if (has_vertex) {

			this->z_list[i] += z_span;
			this->param_list[i] += 1;

			this->frame.addIndex(start_index + 0); this->frame.addIndex(start_index + 1);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 1);
		}
		else {

			this->z_list.erase(this->z_list.begin() + i);
			this->param_list.erase(this->param_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);

	this->face.drawFaces();

	ofSetColor(39);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}