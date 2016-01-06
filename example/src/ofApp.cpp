#include "ofMain.h"

#include "ofxPy.h"

class ofApp : public ofBaseApp
{
public:

	ofxPy::Context py;
	
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofBackground(0);
		
		// setup context
		py.setup();
		
		// setup native binding
		{
			ofxPy::module m("of_bind_test");
			
			m.def("test_fn", [&](){
				cout << "hello from c++ function" << endl;
			});
			
			py.import(m);
		}
		
		// call c++ function
		py.exec("import of_bind_test; of_bind_test.test_fn()");
		
		// append data/python to PYTHONPATH
		py.appendPath(ofToDataPath("python"));
		
		// import and call python script function
		py.exec("import test_script; test_script.my_fn()");
	}

	void update()
	{
		ofSetWindowTitle(ofToString(ofGetFrameRate()));
	}

	void draw()
	{
		// get tuple return value
		auto v = py.eval<ofxPy::tuple>("test_script.get_random()");
		
		// unpack and cast array-like object
		float x = ofxPy::get<float>(v, 0) * ofGetWidth();
		float y = ofxPy::get<float>(v, 1) * ofGetHeight();
		ofDrawRectangle(x, y, 10, 10);
		
		// get function and call with argument
		float s = py.eval("test_script.size_expression").call(ofGetElapsedTimef()).cast<float>();
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), s * 50);
	}

	void keyPressed(int key)
	{
	}

	void keyReleased(int key)
	{
	}

	void mouseMoved(int x, int y)
	{
	}

	void mouseDragged(int x, int y, int button)
	{
	}

	void mousePressed(int x, int y, int button)
	{
	}

	void mouseReleased(int x, int y, int button)
	{
	}

	void windowResized(int w, int h)
	{
	}
};


int main(int argc, const char** argv)
{
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp);
	return 0;
}