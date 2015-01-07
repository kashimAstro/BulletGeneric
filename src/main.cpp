#include "ofMain.h"
#include "bullet.h"
#include "ofxAssimpModelLoader.h"

class MAIN : public ofBaseApp{
	public:
		bullet* bu;
		ofCamera camera;
		ofxAssimpModelLoader model;

		void setup(){
			camera.setPosition(ofVec3f(0, -3.f, -40.f));
		        camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

			bu = new bullet(true,camera);

			bu->initShape("PLANE", ofVec3f(0.,5.5,0.),0., ofVec3f(100.f, 1.f, 100.f));

			bu->initShape("BOX",     ofVec3f(0,0,0), 220., ofVec3f(3.,3.,3.));
			bu->initShape("SPHERE",  ofVec3f(0,5,0), 20., ofVec3f(3.,3.,3.));
			bu->initShape("CONE",    ofVec3f(0,0,-5),230., ofVec3f(3.,3.,3.));
			bu->initShape("CYLINDER",ofVec3f(-5,0,0),20., ofVec3f(3.,3.,3.));
			bu->initShape("CAPSULE", ofVec3f(0,5,0),20.,  ofVec3f(3.,3.,3.));

		        ofQuaternion startRot = ofQuaternion(1., 0., 0., PI);
		        ofVec3f scale(.009, .009, .009);
			ofVec3f startLoc = ofVec3f(0, 0, 0);

			model.loadModel("collada/monster/monster.dae", true);
		        model.setScale(scale.x, scale.y, scale.z);
		        model.setPosition(0, 0, 0);

		        scale = ofVec3f(.0019, .0019, .0019);
			bu->initCustomShape(model, scale, startLoc, startRot);
		}
	
		void exit(){
			bu->clear("ALL");
		}

		void update(){
			bu->update();
		}

		void draw(){
			camera.begin();
				bu->draw(true);
				ofTranslate(bu->getPos("SHAPE"));
				ofRotate(1.,bu->getRot("SHAPE").x, bu->getRot("SHAPE").y, bu->getRot("SHAPE").z);
				model.drawWireframe();
			camera.end();
		}

		void keyPressed(int key){
			if(key == '1')
				bu->clear("BOX");
			if(key == '2')
				bu->clear("SPHERE");
			if(key == '3')
				bu->clear("CAPSULE");
			if(key == '4')
				bu->clear("CONE");
			if(key == '5')
				bu->clear("CYLINDER");
			if(key == '6')
				bu->clear("SHAPE");

		}
};

int main( ){
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new MAIN() );
}
