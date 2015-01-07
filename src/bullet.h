#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxAssimpModelLoader.h"

class xBox : public ofxBulletUserData{
	public:
		xBox(){ }
		string type = "BOX";
                
};

class xSphere : public ofxBulletUserData{
	public:
		xSphere(){ }
		string type = "SPHERE";
                
};

class bullet : public ofBaseApp {

	public :
	
        ofxBulletWorldRigid world;
        ofxBulletBox*      box;
	ofxBulletSphere*   sphere;
       	ofxBulletCone*     cone;
       	ofxBulletCapsule*  capsule;
       	ofxBulletCylinder* cylinder;

	xBox onBox1;
	xBox onBox2;
	xSphere onSphere1;
	xSphere onSphere2;

	ofxBulletBox*      plane;// 

	vector<ofxBulletCustomShape*> shape;
	ofxAssimpModelLoader  assimp;

	int mousePickIndex;

        ofVec3f mousePickPos;
	vector<string> types;

	bullet( bool eventCollision, ofCamera & camera, ofVec3f gravity = ofVec3f(0,25,0) ){
		world.setup();
		world.enableGrabbing();
		if(eventCollision) {
			world.enableCollisionEvents();
			ofAddListener(world.MOUSE_PICK_EVENT, this, &bullet::mousePickEvent);
			ofAddListener(world.COLLISION_EVENT, this, &bullet::onCollision);
		}
		world.setCamera(&camera);
		world.setGravity( gravity );
	}

	void update() {
		world.update();
	}

	void initCustomShape( ofxAssimpModelLoader assimp, ofVec3f scale = ofVec3f(.008,.008,.008),
			      ofVec3f startLoc = ofVec3f(0,0,0), ofQuaternion startRot = ofQuaternion(1., 0., 0., PI), float mass = 3. ) {

                shape.push_back( new ofxBulletCustomShape() );
                for(int i = 0; i < assimp.getNumMeshes(); i++) {
                          shape[i]->addMesh(assimp.getMesh(i), scale, true);
		}
                //shape[i]->init( (btCompoundShape*)shape[0]->getCollisionShape(), shape[0]->getCentroid() );
                shape[0]->create( world.world, startLoc, startRot, mass );
                shape[0]->add();
		types.push_back("SHAPE");
	}


	void initShape( string type, ofVec3f pos = ofVec3f(5,0,0), float mass = .05, ofVec3f size = ofVec3f(0.5,0.5,0.5) ){
		if(type == "BOX"){
		        box = new ofxBulletBox();
		        box->create(world.world, pos, mass, size.x, size.y, size.z);
		        box->add();
			types.push_back(type);
		}
		if(type == "SPHERE"){
			sphere = new ofxBulletSphere();
		        sphere->create(world.world, pos, mass, size.x);
		        sphere->add();
			types.push_back(type);
		}
		if(type == "CONE"){
		        cone = new ofxBulletCone();
		        cone->create(world.world, pos, mass, size.x, size.y);
		        cone->add();
			types.push_back(type);
		}
		if(type == "CAPSULE"){
		        capsule = new ofxBulletCapsule();
		        capsule->create(world.world, pos, mass, size.x, size.y);
		        capsule->add();
			types.push_back(type);
		}
		if(type == "CYLINDER"){
			cylinder = new ofxBulletCylinder();
		        cylinder->create(world.world, pos, mass, size.x, size.y);
		        cylinder->add();
			types.push_back(type);
		}
		if(type == "PLANE"){
			plane = new ofxBulletBox();
			plane->create( world.world, pos, mass, size.x, size.y, size.z );
			plane->setProperties(.25, .95);
		        plane->add();
		}
	}

	ofVec3f getPos(string type){
		if(type == "BOX")
			return box->getPosition();
		if(type == "SPHERE")
			return sphere->getPosition();
		if(type == "CONE")
			return cone->getPosition();
		if(type == "CAPSULE")
			return capsule->getPosition();
		if(type == "CYLINDER")
			return cylinder->getPosition();
		if(type == "SHAPE")
			return shape[0]->getPosition();

	}

	ofVec3f getRot(string type){
		if(type == "BOX")
			return box->getRotationAxis();
		if(type == "SPHERE")
			return sphere->getRotationAxis();
		if(type == "CONE")
			return cone->getRotationAxis();
		if(type == "CAPSULE")
			return capsule->getRotationAxis();
		if(type == "CYLINDER")
			return cylinder->getRotationAxis();
		if(type == "SHAPE")
			return shape[0]->getRotationAxis();

	}


	void draw(bool debug) {
		if(debug) world.drawDebug();
	}
	
	void clear(string type){
		if(type == "BOX")      delete box; 
		if(type == "SPHERE")   delete sphere;
		if(type == "CONE")     delete cone;
		if(type == "CAPSULE")  delete capsule;
		if(type == "CYLINDER") delete cylinder;
		if(type == "SHAPE") {
			 for( int i = 0; i < shape.size(); i++ ){
				shape[i]->removeRigidBody();
			 }
		}
		if(type == "ALL"){
			delete box; 
			delete sphere;
		        delete cone;
		        delete capsule;
		        delete cylinder;
		}
	}

	void onCollision(ofxBulletCollisionData& cdata) {
		cout<<"TypeCollission-box1"<<onBox1.type <<endl;
		cout<<"TypeCollission-box2"<<onBox2.type <<endl;
		cout<<"TypeCollission-sphere1"<<onSphere1.type <<endl;
		cout<<"TypeCollission-sphere2"<<onSphere1.type <<endl;
		if(*plane == cdata) 
			return;
		else{	
			vector<ofVec3f> wcPos = cdata.worldContactPoints1;
			for( int i = 0; i < wcPos.size(); i++ ){
				cout<< "wcX:::" << wcPos[i].x << " - wcY:::" << wcPos[i].y << " - wcZ:::" << wcPos[i].z <<endl;
			}
		}
	}

	void mousePickEvent( ofxBulletMousePickEvent &e ) {
		cout<<"mousepicker"<<endl;
		mousePickIndex = -1;
		for(int i = 0; i < types.size(); i++){
			if(types[i] == "BOX"){
				if(*box == e) {
					mousePickIndex = i;
					mousePickPos = e.pickPosWorld;
					cout<<"types:::"<<types[i]<<" - poser:::"<<mousePickPos<<" - index:::"<<mousePickIndex<<endl;
				}
			}
			if(types[i] == "SPHERE"){
				if(*sphere == e) {
					mousePickIndex = i;
					mousePickPos = e.pickPosWorld;
					cout<<"types:::"<<types[i]<<" - poser:::"<<mousePickPos<<" - index:::"<<mousePickIndex<<endl;
				}
			}
			if(types[i] == "CONE"){
				if(*cone == e) {
					mousePickIndex = i;
					mousePickPos = e.pickPosWorld;
					cout<<"types:::"<<types[i]<<" - poser:::"<<mousePickPos<<" - index:::"<<mousePickIndex<<endl;
				}
			}
			if(types[i] == "CYLINDER"){
				if(*cylinder == e) {
					mousePickIndex = i;
					mousePickPos = e.pickPosWorld;
					cout<<"types:::"<<types[i]<<" - poser:::"<<mousePickPos<<" - index:::"<<mousePickIndex<<endl;
				}
			}
			if(types[i] == "CAPSULE"){
				if(*capsule == e) {
					mousePickIndex = i;
					mousePickPos = e.pickPosWorld;
					cout<<"types:::"<<types[i]<<" - poser:::"<<mousePickPos<<" - index:::"<<mousePickIndex<<endl;
				}
			}
			if(types[i] == "SHAPE"){
				if(*shape[0] == e) {
					mousePickIndex = i;
					mousePickPos = e.pickPosWorld;
					cout<<"types:::"<<types[i]<<" - poser:::"<<mousePickPos<<" - index:::"<<mousePickIndex<<endl;
				}
			}
		}
	}
};
