#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <map>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Loader {
public:
 
  static Loader& getInstance() {
    static Loader *m_loader = new Loader();
    return *m_loader;
  }
  virtual ~Loader();
  void LoadToVAO(std::vector<glm::vec3> vertices, std::vector<glm::vec2> textures,
		 std::vector<glm::vec3> normals, std::vector<int> indices, const std::string& name, std::vector<float> bBox);
  inline void UnbindVAO() { glBindVertexArray(0); }
  void LoadTexture(const std::string& fileName, bool repeat);
  GLuint GetVAO(const std::string& modelName);
  GLuint GetTextureID(const std::string & textureName);
  int GetVertexCount(const std::string& modelName);
  std::vector<float> GetBoundingBox(const std::string& modelName);
protected:
  Loader();
 private:
  std::map<const std::string, std::vector<float>> m_boundingBoxes;
  std::map<const std::string, GLuint> m_vaos;
  std::vector<GLuint> m_vbos;
  std::map<const std::string, GLuint> m_textures;
  std::map<const std::string, int> m_vertSize;
  void StoreBoundingBox(const std::string& name, std::vector<float> bBox);
  GLuint CreateVAO(const std::string& name);
  void StoreDataInAttributeList(GLuint attribNumber, int attribSize, void* data, int dataSize);
  void BindIndicesBuffer(int* indices, int& count);
};


#endif 
