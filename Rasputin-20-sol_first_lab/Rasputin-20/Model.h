#ifndef MODEL_H
#define MODEL_H


#include <GL\GL.h>
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "shader_loader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class Model
{
public:
    
    vector<Mesh> meshes;
    string directory;
        
    Model(string const& path) 
    {
        loadModel(path);
    }
       
    void Draw(Shader_loader& shader, GLuint shader_program, glm::mat4 transformOX1, glm::mat4 transformOX2, glm::mat4 transformOX3)
    {
        for (int i = 0; i < meshes.size(); i++) {
            switch (i){
            case 0: {
                shader.uniform_set_vec(shader_program, "model", 1, glm::value_ptr(transformOX1), GL_FALSE, shader.M4);
                break;
            }

            case 3: {
                glm::mat4 minusTranslateOX2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.1897, 0.359926));
                glm::mat4 TranslateOX2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1.1897, -0.359926));
                glm::mat4 result = transformOX1 * minusTranslateOX2  * transformOX2 * TranslateOX2 ;

                shader.uniform_set_vec(shader_program, "model", 1, glm::value_ptr(result), GL_FALSE, shader.M4);
                break;
            }
 
            case 1: {
                glm::mat4 minusTranslateOX2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.1897, 0.359926));
                glm::mat4 TranslateOX2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1.1897, -0.359926));
                glm::mat4 result1 = minusTranslateOX2 * transformOX2 * TranslateOX2;

                glm::mat4 minusTranslateOX3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 2.88593, 0.461069));
                glm::mat4 TranslateOX3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -2.88593, -0.461069));
                glm::mat4 result = transformOX1 * result1 * minusTranslateOX3 *  transformOX3 * TranslateOX3;

                shader.uniform_set_vec(shader_program, "model", 1, glm::value_ptr(result), GL_FALSE, shader.M4); 
                break;
            }
            default:
                shader.uniform_set_vec(shader_program, "model", 1, glm::value_ptr(glm::mat4(1.0f)), GL_FALSE, shader.M4);
                break;
            }
            meshes[i].Draw(shader, shader_program);
        }
    }

private:
    
    void loadModel(string const& path)
    {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path,
                aiProcess_CalcTangentSpace |
                aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_SortByPType);

            if (nullptr == scene) {
                cout << "ERROR: IMPORT FAILED", importer.GetErrorString();
            }

            processNode(scene->mRootNode, scene); 
                //DoTheSceneProcessing(scene);
  
    }
        
    void processNode(aiNode* node, const aiScene* scene) {
        for (int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        vector<Vertex> vertices;
        vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            vertices.push_back(vertex); 
        }

 
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        return Mesh(vertices, indices);
    }
};

#endif


