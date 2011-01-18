#pragma once

#ifndef __YAML_SERIALIZABLE_H__
#define __YAML_SERIALIZABLE_H__

#include <stdexcept>
#include <string>

namespace YAML
{
    /** A mixin to add YAML serialization capability to any class.
     
        Simply inherit from this class, call YAML_SERIALIZABLE_AUTO 
        and then implement overrides for the pure virtuals
        emit_members() and load_members().  Also make sure you have
        a public default constructor defined to allow for instantiating
        std containers of your class.

      <pre>
     
      Example usage: 
      class my_class : public my_parent, public YAML::Serializable {
           public:
               YAML_SERIALIZABLE_AUTO(my_class)
           protected:
              my_class() x(0),y(0) { }  // must provide a default constructor
              double x;
              double y;
              vector<class_z> z_list;
               void emit_members(YAML::Emitter& out) {
                       YAML_EMIT_PARENT_MEMBERS(out,my_parent);
                       YAML_EMIT_MEMBER(out,x);
                       YAML_EMIT_MEMBER(out,y);
                       YAML_EMIT_MEMBER(out,z_list);
               }
               void load_members(YAML::Node& doc) {
                   YAML_LOAD_PARENT_MEMBERS(doc,my_parent);
                   YAML_LOAD_MEMBER(doc,x);
                   YAML_LOAD_MEMBER(doc,y);
                   YAML_LOAD_MEMBER(doc,z_list);
               }
               ...
      };
      ...
      YAML::Parser parser(instream);
      YAML::Node doc;
      parser.GetNextDocument(doc);
      my_class my_obj;
      doc >> my_obj;
      YAML::Emitter;
      emitter << my_obj;
      </pre>

    **/
    class Serializable {
    #define CLASS_TAG "Class"
            public:

                /** Generate yaml to completely encode the object.
                 *
                 * This implementation should work for most cases but it is
                 * virtual just in case you have some situation that this does
                 * not cover.
                 * Most classes will simply need to override emit_members()
                 * \param out The object in which output is collected.
                **/
                virtual void emit_yaml(YAML::Emitter& out) const {
                    out << YAML::BeginMap;
                    std::string my_class_name = class_name();
                    if (my_class_name.size()>0) {
                        out << YAML::Key << CLASS_TAG << YAML::Value << my_class_name;
                    }
                    this->emit_members(out);
                    out << YAML::EndMap;
                }

                /** Load the object from a YAML node
                 *
                 * This implementation should work for most cases but it is
                 * virtual just in case you have some situation that this does
                 * not cover.
                 * Most classes will simply need to override load_members()
                 * \param doc The YAML node from which to load
                **/
                virtual void load_yaml(const YAML::Node& doc) {
                    if (const YAML::Node *pClass = doc.FindValue(CLASS_TAG)) {
                        std::string found_class_name;
                        *pClass >> found_class_name;
                    #ifdef EASSERT
                        EASSERT(found_class_name == this->class_name(),
                                "yamlized class name \"" << found_class_name 
                                << "\" does not match current class name: " 
                                << this->class_name());
                    #endif
                    }
                    load_members(doc);
                }

                // just to keep the compiler from whining
                 virtual ~Serializable() {}

            protected:
                /** Load all members of "this" from the specified YAML node
                  
                  This pure virtual method should be overridden in all
                  descendant classes and is effectively the heart of the
                  YAML marshalling system.
                 
                  \param doc The YAML node from which to load the data.
                  \sa load_from_yaml()
                  \sa YAML_LOAD_MEMBER(doc,member)
                  \sa YAML_LOAD_PARENT_MEMBERS(doc,parent_class_name)
                **/
                virtual void load_members(const YAML::Node& doc) = 0;

                /** Emit all members of "this" via the specified YAML emitter
                  
                  This pure virtual method should be overridden in all
                  descendant classes and is effectively the heart of the
                  YAML marshalling system.
                 
                  \param out The YAML emitter to be used for emitting.
                  \sa emit_yaml()
                  \sa YAML_EMIT_MEMBER(emitter,member)
                  \sa YAML_EMIT_PARENT_MEMBERS(emitter,parent_class_name)
                **/
                virtual void emit_members(YAML::Emitter& out) const = 0;

                /// Name of class stored for info purposes in yaml output
                //  All subclasses should override this to return their name.
                //  This is automamtically taken care of if you use
                //  YAML_SERIALIZABLE_AUTO
                virtual std::string class_name() const { return ""; }

    };


    /** \def YAML_SERIALIZABLE_AUTO(my_class_name)
    Autogenerate the template code for YAML input and output operators as well
    as a constructor from YAML for any descendant class of YAML::Serializable.
    This should be invoked in the public declarations section of 
    every child of YAML::Serializable.
    \param my_class_name The name of the class for iwhich the code is generated.
    <pre>

    Example:
    class me : public my_dad {
        public:
        YAML_SERIALIZABLE_AUTO(me)
        ...
    };

    </pre>
    \sa YAML::Serializable
    \sa YAML::Serializable::emit_members()
    \sa YAML_SERIALIZABLE_AUTO_DISAMBIGUATED(my_class,preferred_parent)
    **/
    #define YAML_SERIALIZABLE_AUTO(my_class) \
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const my_class &me) { me.emit_yaml(out); return out; }\
    friend void operator >>(const YAML::Node& doc, my_class& me) { me.load_yaml(doc); }\
    my_class(YAML::Node& doc) { doc >> *this; }\
    std::string class_name() const { return #my_class; }

    /** \def YAML_SERIALIZABLE_AUTO_DISAMBIGUATED(my_class_name, preferred_parent)
    A special version of YAML_SERIALIZABLE_AUTO() used when multiple
    ancestors of a class inherit from YAML::Serializable.

    This should be invoked in the public declarations section of 
    every child inherting from multiple YAML::Serializable classes.
    \param my_class_name The name of the class for iwhich the code is generated.
    \param preferred_parent The name of the parent class whose serialization methods will be invoked
    <pre>

    Example:
    class me : public my_dad, public my_mom {
        public:
        YAML_SERIALIZABLE_AUTO_DISAMBIGUATED(me, my_mom)
        ...
    };

    </pre>
    \sa YAML::Serializable
    \sa YAML::Serializable::emit_members()
    \sa YAML_SERIALIZABLE_AUTO(class_name)
    **/
    #define YAML_SERIALIZABLE_AUTO_DISAMBIGUATED(my_class,preferred_parent) \
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const my_class &me) { const preferred_parent* pp = &me;  pp->emit_yaml(out); return out; }\
    friend void operator >>(const YAML::Node& doc, my_class& me) { preferred_parent* pp = &me; pp->load_yaml(doc); }\
    my_class(YAML::Node& doc) { doc >> *this; }\
    std::string class_name() const { return #my_class; }

    /** \def YAML_OSTREAM(my_class_name)
    Autogenerate the template code for an ostream operator<< that spews YAML
    for a specific class.

    There is no corresponding automatic istream operator>> because
    YAML::Parser(istream) slurps all documents in the stream, making expressions
    like this impossible to implement:  cin >> obj1 >> obj2;
    So for input, you'll just have to put up with repeatedly calling 
    parser.GetNextDocument(node) and then node >> objN.
    <pre>

    Example:
    class me : public my_dad, public my_mom {
        public:
        YAML_SERIALIZABLE_AUTO(me)
        YAML_OSTREAM(me)
        ...
    };
    ...

    me obj;
    cout << obj;   // outputs YAMLized version of obj
    </pre>

    **/
    #define YAML_OSTREAM(my_class) \
    friend std::ostream& operator<<(std::ostream& out, const my_class &me) {\
        YAML::Emitter emitter;\
        me.emit_yaml(emitter);\
        out << emitter.c_str();\
        return out; }



    /** \def YAML_EMIT_PARENT_MEMBERS(emitter, parent_class_name)
    Autogenerate the code to include all of a class's parent's members
    in the output when implementing emit_members.  Typically used
    in class-specific overrides of emit_members()
    \param emitter A YAML::Emitter to which the output will be written
    \param parent_class_name The name of a parent class of "this"
    <pre>

    Example:
    class me : public my_dad, public my_mom {
        public:
        YAML_SERIALIZABLE_AUTO(me)
        protected:
        int _max_values;
        vector<double> _values;
        void emit_members(YAML::Emitter &out) const {
            YAML_EMIT_PARENT_MEMBERS(out, my_dad);
            YAML_EMIT_PARENT_MEMBERS(out, my_mom);
            YAML_EMIT_MEMBER(out,_max_values);
            YAML_EMIT_MEMBER(out,_values);
        }
        ...
    };

    </pre>
    \sa YAML::Serializable
    \sa YAML::Serializable::emit_members()
    **/
    #define YAML_EMIT_PARENT_MEMBERS(emitter,parent_class_name) \
    {\
        emitter << YAML::Key << #parent_class_name << YAML::Value << YAML::BeginMap;\
        parent_class_name::emit_members(emitter);\
        out << YAML::EndMap;\
    }


    /** \def YAML_EMIT_MEMBER(emitter, member)
    Output a member of "this" as YAML to the emitter.  Typically used
    in class-specific overrides of emit_members()
    \param emitter A YAML::Emitter to which the output will be written
    \param member The name of a member of "this" to be saved as YAML.
    <pre>

    Example:
    void emit_members(YAML::Emitter &out) const {
        YAML_EMIT_MEMBER(out,_max_values);
        YAML_EMIT_MEMBER(out,_values);
    }

    </pre>
    \sa YAML::Serializable
    \sa YAML::Serializable::emit_members()
    **/
    #define YAML_EMIT_MEMBER(emitter,member) emitter << YAML::Key << #member << YAML::Value << this->member
    #define YAML_EMIT(emitter,member) emitter << YAML::Key << #member << YAML::Value << member

    /** \def YAML_LOAD_PARENT_MEMBERS(doc, parent_class_name)
    Autogenerate the code to load all of a class's parent's members
    from a YAML node when implementing load_members.  Typically used
    in class-specific overrides of load_members()
    \param doc A YAML::Node from which to load the parent's members
    \param parent_class_name The name of a parent class of "this"
    <pre>

    Example:
    class me : public my_dad, public my_mom {
        public:
        YAML_SERIALIZABLE_AUTO(me)
        protected:
        int _max_values;
        vector<double> _values;
        void load_members(YAML::Node &doc) const {
            YAML_LOAD_PARENT_MEMBERS(doc, my_dad);
            YAML_LOAD_PARENT_MEMBERS(doc, my_mom);
            YAML_LOAD_MEMBER(out,_max_values);
            YAML_LOAD_MEMBER(out,_values);
        }
        ...
    };

    </pre>
    \sa YAML::Serializable
    \sa YAML::Serializable::load_members()
    **/
    #define YAML_LOAD_PARENT_MEMBERS(doc,parent_class_name) \
        if (const YAML::Node *pparent = doc.FindValue(#parent_class_name)) {\
            parent_class_name::load_members(*pparent);\
        } else {\
            throw std::runtime_error("could not find parent class in YAML");\
        }

    /** \def YAML_LOAD_MEMBER(doc, member)
    Load  a member of "this" from YAML in the YAML::Node doc.
    Typically used in class-specific overrides of load_members()
    \param doc The YAML::Node from which the data will be extracted
    \param member The name of a member of "this" to be initialized from the YAML data.
    <pre>

    Example:
    void load_members(const YAML::Node& doc) const {
        YAML_LOAD_MEMBER(doc,_max_values);
        YAML_LOAD_MEMBER(doc,_values);
    }
    </pre>
    \sa YAML::Serializable
    \sa YAML::Serializable::load_members()
    **/
    #define YAML_LOAD_MEMBER(doc,member) doc[#member] >> this->member
    #define YAML_LOAD(doc,member) doc[#member] >> member
}

#endif // __YAML_SERIALIZABLE_H__

