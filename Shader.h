#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Class used to store and manage our Shader Programs ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Shader
{
public:
    unsigned int ID;
    ///////////////////////// Constructor Function ////////////////////////////////////////////////
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // Variables used to store infor about our two programs
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;


        // code to allow istreams to throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open our Shader Files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close our shader files
            vShaderFile.close();
            fShaderFile.close();
            // store our streams as strings
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        //if an error is thrown, catch it here
        catch (std::ifstream::failure& e)
        {
            //print the thrown exception
            std::cout << "Error occurred while attempting to read a Shader File:" << e.what() << std::endl;
        }

        //Convert our ShaderCode Strings into cStrings.
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        //Declare our Shaders
        unsigned int vertex, fragment;
        // create Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        //Specify where Shader Code is located
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        //Compile Shader Code
        glCompileShader(vertex);
        //Check for Errors
        checkCompileErrors(vertex, "VERTEX");
        // Create fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        //Specify where Shader Code is located
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        //Compile Shader Code
        glCompileShader(fragment);
        //Check for Errors
        checkCompileErrors(fragment, "FRAGMENT");
        
        //Create actual Program
        ID = glCreateProgram();
        //attach Vertex Shader
        glAttachShader(ID, vertex);
        //Attach Fragment Shader
        glAttachShader(ID, fragment);
        //Link Program
        glLinkProgram(ID);
        //Check for errors
        checkCompileErrors(ID, "PROGRAM");
        // Delete our Shader Proograms, now that they are already linked
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }


    ////////////////////////////////////////// Set this as Active Shader ////////////////////////////////////////////////////////
    void use()
    {
        glUseProgram(ID);
    }
    ///////////////////////////////////////// Set a Specific Bool       /////////////////////////////////////////////////////////
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    ///////////////////////////////////////// Set a Specific Int        /////////////////////////////////////////////////////////
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    ///////////////////////////////////////// Set a Specific Float      /////////////////////////////////////////////////////////
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec2(const std::string& name, float valueA, float valueB)
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), valueA, valueB);
    }

private:
    ///////////////////////////////////////// Check for specific Errors ///////////////////////////////////////
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Error while compiling Shader type: " << type << "\n" << infoLog << "\n" << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Error While Linking Program. type:" << type << "\n" << infoLog << "\n" <<  std::endl;
            }
        }
    }
};