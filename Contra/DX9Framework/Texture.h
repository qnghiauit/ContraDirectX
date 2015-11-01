//==============================
// Last change: 28/9/2014
// T6_Framework
// T6_Texture v1.1
// Each object can store an image and be able to draw it on screen.
// 28/9/2014: remove Name, and change Path type to const tchar*.
//==============================
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#define CACHE_SIZE 10

#include <d3d9.h>
#include <d3dx9.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <exception>

#include "DefaultConstant.h"
#include "DefaultStructure.h"
#include "DefaultMethods.h"

using namespace std;


class Texture
{
public:
	Texture(){};
	Texture(const tchar* texturePath);
	Texture(const Texture& sourceTexture);
	virtual ~Texture(void);

	static HRESULT	initializeHandler(const LPD3DXSPRITE& handler);
	static HRESULT	releaseHandler();
	static void		setPositioncConvertMatrix(const D3DXMATRIX& newMatrix);

	LPD3DXSPRITE	getHandler()	const;
	const tchar*	getPath()		const;
	D3DXIMAGE_INFO	getInfo()		const;
	bool			isLoaded()		const;

	virtual	Texture*		clone()				const;
	virtual unsigned int	getCurrentFrame()	const;
	virtual unsigned int	getTotalFrames()	const;
	virtual Size			getFrameSize();
	virtual void			reset();
	virtual bool			isComplete();

	virtual void loadImageFromStoredInfo();
			void convertVisiblePercent(float width, float height, RECT& sourceRECT, D3DXVECTOR3& position, bool xAxisTranslate, D3DXVECTOR2 scale);

	void drawImage (D3DXVECTOR3			pos			= D3DXVECTOR3(0,0,0.5),
					RECT*				srcRect		= 0,
					D3DXVECTOR2			scale		= SCALE_NO,
					D3DXVECTOR2			translate	= TRANSLATE_NO,
					const DrawCenter&	drawCenter	= DrawCenter::CENTER,
					float				angle		= ANGLE_NO,
					D3DCOLOR			color		= 0xFFFFFFFF,
					bool				converted	= true);

	void drawImage(D3DXVECTOR3			pos,
					RECT*				srcRect,
					D3DXVECTOR2			scale,
					D3DXVECTOR2			translate,
					D3DXVECTOR3			drawCenter,
					float				angle = ANGLE_NO,
					D3DCOLOR			color = 0xFFFFFFFF,
					bool				converted = true);

	virtual bool render(double		passedGameTime = 0.0,
						D3DXVECTOR3	position = D3DXVECTOR3(0, 0, 0.5),
						float		angle = ANGLE_NO,
						D3DXVECTOR2	scale = SCALE_NO,
						D3DXVECTOR2	translate = TRANSLATE_NO,
						DrawCenter	drawCenter = DrawCenter::CENTER,
						D3DCOLOR	color = D3DCOLOR_XRGB(255, 255, 255),
						int			from = 0,
						int			to = -1,
						RECT*		sourceRect = nullptr);

	virtual bool render(
						double		passedGameTime = 0.0,
						D3DXVECTOR3	position = D3DXVECTOR3(0, 0, 0.5),
						float		visiblePercent_width = 1.0f,
						float		visiblePercent_height = 1.0f,
						bool		isTranslateXAxis = true,
						D3DXVECTOR2	scale = SCALE_NO,
						DrawCenter	drawCenter = DrawCenter::CENTER,
						int			from = 0,
						int			to = -1);

	Texture& operator=(const Texture& sourceTexture);

protected:
	D3DXVECTOR3 computeCenter(const DrawCenter& drawCenter);

private:
	void		releaseTexture();

	void		recorrectImagePositionBaseOnDrawCenter(D3DXVECTOR3& pos, const DrawCenter& drawCenter,
														const int& destWidth, const int& destHeight);

	void		transformImage(const float& angle, const D3DXVECTOR2& scale, const D3DXVECTOR2& translate,
								const D3DXVECTOR3& position, const D3DMATRIX& originalStatus);

	Texture* tryLoadFromCache();

private:
	static LPD3DXSPRITE			_handler;
	static D3DXMATRIX			_cameraConvertMatrix;
	static LPDIRECT3DDEVICE9	_d3dDevice;
	static vector<Texture*>	_loadedTexture;

	tstring*			_path;
	D3DXIMAGE_INFO*		_info;
	LPDIRECT3DTEXTURE9*	_texture;
};

#endif