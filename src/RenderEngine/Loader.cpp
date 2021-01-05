#include "Loader.h"
#include <iostream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Loader::Loader()
{
  // Initialize the storage arrays
  m_vaos.clear();
  m_vbos.clear();
  m_textures.clear();
  m_vertSize.clear();
}


Loader::~Loader()
{
  // Loop through all the storage arrays and delete OpenGL variables

  while (m_vbos.size() > 0)
    {
      glDeleteBuffers(1, &m_vbos.back());
      m_vbos.pop_back();
    }
  /*
  while (m_vaos.size() > 0)
    {
      glDeleteVertexArrays(1, &m_vaos.back());
      m_vaos.pop_back();
    }
  */
  /*  while (m_textures.size() > 0)
    {
      glDeleteTextures(1, &m_textures.back());
      m_textures.pop_back();
      }*/
}


void Loader::LoadToVAO(std::vector<glm::vec3> vertices, std::vector<glm::vec2> textures, std::vector<glm::vec3> normals, std::vector<int> indices, const std::string& name, std::vector<float> bBox)
{
  // create a new VAO
  GLuint vaoID = CreateVAO(name);
  int indicesSize = indices.size();
  m_vertSize.insert(std::pair<const std::string, int>(name, indicesSize));
  BindIndicesBuffer(indices.data(), indicesSize);
  // Store the data in attribute lists
  StoreBoundingBox(name, bBox);
  StoreDataInAttributeList(0, 3, &vertices[0], vertices.size() * sizeof(glm::vec3));
  StoreDataInAttributeList(1, 2, &textures[0], textures.size() * sizeof(glm::vec2));
  StoreDataInAttributeList(2, 3, &normals[0], normals.size() * sizeof(glm::vec3));
  UnbindVAO();
  
}

std::vector<float> Loader::GetBoundingBox(const std::string& modelName) {
  return m_boundingBoxes[modelName];
}

GLuint Loader::GetVAO(const std::string& modelName) {
  return m_vaos[modelName];
}

GLuint Loader::GetTextureID(const std::string& textureName) {
  return m_textures[textureName];
}

int Loader::GetVertexCount(const std::string& modelName) {
  return m_vertSize[modelName];
}

void Loader::LoadTexture(const std::string& fileName, bool repeat)
{
  GLuint texture;
  int width, height, numComponents;
  // Load image data
  stbi_uc* imageData = stbi_load(("../res/textures/" + fileName + ".png").c_str(), &width, &height, &numComponents, 4);

  if (imageData == NULL)
    std::cerr << "ERROR: texture loading failed for " << fileName << std::endl;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  float aniso = 16.0f;

  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);


  // How OpenGL will fill an area that's to big or to small
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  // Allow repeating textures for terrain
  if(!repeat) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
    
  // Store the OpenGL texture data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
  
  glGenerateMipmap(GL_TEXTURE_2D);
  // Store the buffer in the list
  m_textures.insert(std::pair<const std::string, GLuint>(fileName,texture));

  // Unload image data
  stbi_image_free(imageData);

  //return texture;
}


GLuint Loader::CreateVAO(const std::string& name)
{
  GLuint vaoID;
  // Create a new VAO
  glGenVertexArrays(1, &vaoID);
  // Store the vao in the list
  m_vaos.insert(std::pair<const std::string,GLuint>(name,vaoID));
  // Bind the VAO to use it
  glBindVertexArray(vaoID);
  return vaoID;
}

void Loader::StoreBoundingBox(const std::string& name, std::vector<float> bBox) {
  m_boundingBoxes.insert(std::pair<const std::string, std::vector<float>>(name, bBox));
}


void Loader::StoreDataInAttributeList(GLuint attribNumber, int attribSize, void* data, int dataSize)
{
  GLuint vboID;
  // Create a new buffer
  glGenBuffers(1, &vboID);
  // Store the buffer in the list
  m_vbos.push_back(vboID);
  // Bind the buffer to use it
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  // Store the data in the buffer
  glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
  // Tell OpenGL how and where to store this VBO in the VAO
  glVertexAttribPointer(attribNumber, attribSize, GL_FLOAT, GL_FALSE, 0, nullptr);
}


void Loader::BindIndicesBuffer(int* indices, int& count)
{
  GLuint vboID;
  // Generate a buffer and bind it for use
  glGenBuffers(1, &vboID);
  // Store the buffer in the list
  m_vbos.push_back(vboID);
  // Bind the buffer to use it
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
  // Store the indices in the buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * count, indices, GL_STATIC_DRAW);
}
