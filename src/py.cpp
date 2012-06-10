#ifdef py
using namespace boost::python;


void init_py()
{
    Py_InitializeEx(0);
}

void baf()
{
    printf("BAF\n");
}
/*
class World
{
    

};


World world;

void get_world()
{
    object get_iterator = std::iterator<std::vector<obj*> >();
    object iter = get_iterator(world);
    
}
*/
BOOST_PYTHON_MODULE(lemon)
{
    def("baf", baf);
    class_<Settingz>("settings")
    .def_readwrite("line_antialiasing",&settingz.line_antialiasing);
//    class_<World>("local")
//    .def("__iter__", iterator<obj*>());
}

void greet()
{ 
  initlemon();
  boost::python::object main = boost::python::import("__main__");
  boost::python::object global(main.attr("__dict__"));
  try
  {
    boost::python::object result = boost::python::exec(
    "import lemon                   \n"
    "def greet():                   \n"
    "  lemon.baf()                  \n"
    "#  lemon.settings.line_antialiasing = True                 \n"
    "  return 'Hello from Python!'  \n",
    global, global);

  // Create a reference to it.
  boost::python::object greet = global["greet"];
  std::string message = boost::python::extract<std::string>(greet());
  std::cout << message << std::endl;
  }
  catch (boost::python::error_already_set &e)
  {
	PyErr_Print();
  }
}

void kill_py()
{
	/*Bugs and caveats: The destruction of modules and objects in modules is done in random order; this may cause destructors (__del__() methods) to fail when they depend on other objects (even functions) or modules. Dynamically loaded extension modules loaded by Python are not unloaded. Small amounts of memory allocated by the Python interpreter may not be freed (if you find a leak, please report it). Memory tied up in circular references between objects is not freed. Some memory allocated by extension modules may not be freed. Some extensions may not work properly if their initialization routine is called more than once; this can happen if an application calls Py_Initialize() and Py_Finalize() more than once.*/


	Py_Finalize();
	
}
#endif
