#include "BrowseMode.hpp"

#include <Draw.hpp>
#include <Character/pose_utils.hpp>
#include <Library/Library.hpp>
#include <Character/skin_utils.hpp>

#include <Graphics/Graphics.hpp>
#include <Graphics/Font.hpp>
#include <Vector/VectorGL.hpp>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using std::string;

using std::ostringstream;

using std::cout;
using std::cerr;
using std::endl;

using std::ofstream;

//static const double M_PI = 3.14159265;

BrowseMode::BrowseMode() {
	camera = make_vector(0.0f, 50.0f, 40.0f);
	target = make_vector(0.0f, 10.0f, -20.0f);
	track = false;
	current_pose.clear();
	current_motion = 0;
	time = 0.0f;
	play_speed = 0.0f;

	cloth.reset();
  cloth.draw();
	//cloth.simulation_step();
//	flow.test1();
//	flo3.test1();
	

}

BrowseMode::~BrowseMode() {
}

void BrowseMode::update(float const elapsed_time) {
	assert(current_motion < Library::motion_count());
	Library::Motion const &motion = Library::motion(current_motion);
	if (motion.frames() == 0) {
		cerr << "Motion from " << motion.filename << " has zero frames." << endl;
		return;
	}
	time = fmodf(elapsed_time * play_speed + time, motion.length());
	unsigned int frame = (unsigned int)(time / motion.skeleton->timestep);
	if (frame >= motion.frames()) frame = motion.frames() - 1;

	//motion.get_pose(frame, current_pose);
	motion.get_pose(1, current_pose);///delete this line and uncomment last line if you want the character to move again

	if (track) {
		Vector3f delta = current_pose.root_position - target;
		target += delta;
		camera += delta;
	}

	cloth.simulation_step();
	cout << "frame: " << frame<<endl;
//	flow.flow_step();
//	flo3.flo3_step();

	
	
	
}

void BrowseMode::handle_event(SDL_Event const &event) {
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
		track = !track;
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		quit_flag = true;
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_PAGEUP) {
		current_motion += 1;
		if (current_motion >= Library::motion_count()) {
			current_motion = 0;
		}
		time = 0.0f;
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_PAGEDOWN) {
		current_motion -= 1;
		if (current_motion >= Library::motion_count()) {
			current_motion = Library::motion_count() - 1;
		}
		time = 0.0f;
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
		if (play_speed == 1.0f) play_speed = 0.0f;
		else if (play_speed == 0.0f) play_speed = 1.0f;
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d) {
		for (unsigned int m = 0; m < Library::motion_count(); ++m) {
			Library::Motion const &motion = Library::motion(m);
			cout << "Dumpping '" << motion.filename + ".global" << "'" << endl;
			ofstream out((motion.filename + ".global").c_str());
			out << "\"position.x\", \"position.z\", \"position.yaw\", \"root.x\", \"root.y\", \"root.z\"";
			for (unsigned int b = 0; b < motion.skeleton->bones.size(); ++b) {
				out << ", ";
				out << '"' << motion.skeleton->bones[b].name << ".x" << '"';
				out << ", ";
				out << '"' << motion.skeleton->bones[b].name << ".y" << '"';
				out << ", ";
				out << '"' << motion.skeleton->bones[b].name << ".z" << '"';
			}
			out << endl;
			for (unsigned int f = 0; f < motion.frames(); ++f) {
				Character::WorldBones w;
				Character::Pose p;
				motion.get_local_pose(f, p);
				Character::get_world_bones(p, w);
				{
					Character::StateDelta delta;
					motion.get_delta(0, f, delta);
					out << delta.position.x << ", " << delta.position.z << ", " << delta.orientation << ", " << p.root_position.x << ", " << p.root_position.y << ", " << p.root_position.z;
				}
				for (unsigned int b = 0; b < w.tips.size(); ++b) {
					for (unsigned int c = 0; c < 3; ++c) {
						out << ", ";
						out << w.tips[b].c[c];
					}
				}
				out << endl;
			}
		}
	}

	//rotate view:
	if (event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))) {
		Vector3f vec = camera - target;
		float len = length(vec);
		float theta_yaw = atan2(vec.z, vec.x);
		float theta_pitch = atan2(vec.y, sqrt(vec.x * vec.x + vec.z * vec.z));
		theta_yaw += event.motion.xrel / float(Graphics::screen_x) / len * 100;
		theta_pitch += event.motion.yrel / float(Graphics::screen_x) / len * 100;
		if (theta_pitch > 0.4 * M_PI) theta_pitch = 0.4 * M_PI;
		if (theta_pitch <-0.4 * M_PI) theta_pitch =-0.4 * M_PI;

		camera = make_vector(cosf(theta_yaw)*cosf(theta_pitch),sinf(theta_pitch),sinf(theta_yaw)*cosf(theta_pitch)) * len + target;
	}
	//pan view:
	if (event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE))) {
		Vector3f to = normalize(camera - target);
		Vector3f right = normalize(cross_product(to, make_vector< float >(0,1,0)));
		Vector3f up = -cross_product(to, right);
		float len = length(camera - target);
		camera += right * event.motion.xrel / float(Graphics::screen_x) * len;
		camera += up * event.motion.yrel / float(Graphics::screen_x) * len;
		target += right * event.motion.xrel / float(Graphics::screen_x) * len;
		target += up * event.motion.yrel / float(Graphics::screen_x) * len;
	}
	//zoom view:
	if (event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT))) {
		Vector3f vec = camera - target;
		float len = length(vec);
		len *= pow(2, event.motion.yrel / float(Graphics::screen_x) * 10);
		if (len < 1) len = 1;
		if (len > 100) len = 100;
		camera = normalize(camera - target) * len + target;
	}
}

void BrowseMode::draw() {
const int FloorSize = 100;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

//	gluPerspective(60.0, Graphics::aspect(), 0.1, 10.0 * FloorSize);
	gluPerspective(60.0, 8.f/6.f, 0.1, 10.0 * FloorSize);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	{
		float y = camera.y;
		if (y < 0.1f) y = 0.1f;
		gluLookAt(camera.x, y, camera.z, target.x, target.y, target.z, 0, 1, 0);
	}
		flo3.draw();
	glDisable(GL_DEPTH_TEST);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	//--- now some info overlay ---
	

	Graphics::gl_errors("BrowseMode::draw");
}
