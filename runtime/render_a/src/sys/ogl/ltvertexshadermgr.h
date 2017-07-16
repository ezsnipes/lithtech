#ifndef __LTVERTEXSHADERMGR_H__
#define __LTVERTEXSHADERMGR_H__

#define __OPENGL 3.3

#include "ltbasedefs.h"
#include "ltidtoobjecttable.h"

struct OGLVertexElement {
    float x,y,z,w;
};

struct OGLVertexDeclaration
{};

class VertexShader : public LTVertexShader
{
public:
	VertexShader()
		: m_bCompileShader(false),
	      m_pByteCode(nullptr),
		  m_ByteCodeSize(0),
		  m_pShader(nullptr),
		  m_pVertexElements(nullptr),
		  m_pDeclaration(nullptr)
	{
	}

	~VertexShader()
	{
		Term();
	}

	// initialize
	bool							Init(ILTStream *pStream, const OGLVertexElement *pVertexElements,
										 uint32 VertexElementsSize, bool bCompileShader);

	// terminate
	void							Term();

	// recreate the shader interfaces
	bool							Recreate();

	// free the shader interfaces
	void							FreeDeviceObject();

	// is the shader valid
	virtual bool					IsValidShader() const					{ return (NULL != m_pShader && NULL != m_pDeclaration); }

	// id
	void							SetID(uint32 ShaderID)						{ m_ShaderID = ShaderID; }

	// name
	void							SetName(const char *pName);

	// next pointer
	void							SetNext(VertexShader *pNext)		{ m_pNext = pNext; }

	// byte code
	uint8*							GetByteCode()							{ return m_pByteCode; }

	// get the values in a constant register
	virtual bool					GetConstant(unsigned RegisterNum, float *pf0, float *pf1, float *pf2, float *pf3);

	// set the values in a constant register
	virtual bool					SetConstant(unsigned RegisterNum, float f0, float f1, float f2, float f3);

	// copies the values in the given matrix to the four constant registers starting at RegisterNum
	virtual bool					SetConstant(unsigned RegisterNum, const LTMatrix &Matrix);

	// constants
	virtual float*					GetConstants()							{ return m_Constants; }
    
private:

	bool							m_bCompileShader;		// this flag specifies whether the shader is already compiled

	uint8*							m_pByteCode;			// byte code loaded in from the file
	unsigned						m_ByteCodeSize;			// size of byte code array

	VertexShader*			        m_pShader;				// d3d shader interface

	OGLVertexElement*				m_pVertexElements;		// An array of vertex elements used to create the vertex shader declaration
	OGLVertexDeclaration*           m_pDeclaration;			// d3d vertex shader input declaration interface

	float							m_Constants[LTVertexShader::MAX_CONSTANT_REGISTERS*4];			// user-defined constants
};

class LTVertexShaderMgr {
public:
    static LTVertexShaderMgr& GetSingleton();
    uint32 AddVertexShader(ILTStream* file, const char* sname, int, OGLVertexElement*, uint32&, bool&);
    void RemoveVertexShader(uint32 id);
    void RemoveAllVertexShaders();
    VertexShader* GetVertexShader(uint32 id);
};

#endif // __LTVERTEXSHADERMGR_H__