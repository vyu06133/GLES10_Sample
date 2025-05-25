#include "framework.h"
#include <GLES/gl.h>
#include "TaskSystem.h"

#include "boids.h"

int MainWnd = 0;
int WndWidth = 0;
int WndHeight = 0;

#define	NUMBER				500
#define	RANGE				20
#define	RANGE_HEIGHT		10
#define	RANGE_INIT			5

#define	ANGLE_STEP			2.0f
#define	ANGLE_MAX			30.0f
#define	ANGLE_MIN			-30.0f
#define	ANGLE_THRESHOLD		30.0f

#define	SPEED_STEP			0.002f
#define	SPEED_MAX			0.05f
#define	SPEED_MIN			0.02f

#define	DISTANCE_MAX		107.5f
#define	DISTANCE_MIN		1.5f
#define	DISTANCE_THRESHOLD	15.0f

#define	OBJECT		10



float	randu( void )
{
	return ( (float)rand() / (float)RAND_MAX );
}

float extent( const glm::vec3& v1, const glm::vec3& v2 )
{
	return (float)( (v1 - v2).length());
}

float lengthSquare(const glm::vec3& v1, const glm::vec3& v2)
{
	return (v1.x-v2.x) * (v1.x-v2.x) + (v1.y-v2.y) * (v1.y-v2.y) + (v1.z-v2.z) * (v1.z-v2.z);
}

float step( float a )
{
	while ( a < -180.0f )	a += 360.0f;
	while ( a >= 180.0f )	a -= 360.0f;
	if ( a < -ANGLE_STEP )	a = -ANGLE_STEP;
	if ( a > ANGLE_STEP )	a = ANGLE_STEP;
	return ( a );
}

float	angle(float a )
{
	while ( a < -180.0f )	a += 360.0f;
	while ( a >= 180.0f )	a -= 360.0f;
	return ( a );
}

glm::vec3	calcDirection( const glm::vec3& from, const glm::vec3& to )
{
	vec3	d;
	vec3	r;
	
	d = to;
	d -= from;
	r.x = glm::degrees( -atan2f( d.y, sqrtf(d.z*d.z + d.x*d.x) ) );
	r.y = glm::degrees( atan2f( d.x, d.z ) );
	r.z = 0.0f;
	return ( r );
}

glm::vec3	calcForward( const glm::vec3& dir )
{
	vec3	f, g;
	float	c, s;
	
	f.x = 0.0f;
	f.y = 0.0f;
	f.z = 1.0f;
	
	// pitch
	c = cosf( glm::radians( dir.x ) );
	s = sinf(glm::radians( dir.x ) );
	g.x = f.x;
	g.y = f.y * c - f.z * s;
	g.z = f.y * s + f.z * c;
	
	// heading
	c = cosf(glm::radians( dir.y ) );
	s = sinf(glm::radians( dir.y ) );
	f.x = g.z * s + g.x * c;
	f.y = g.y;
	f.z = g.z * c - g.x * s;
	
	return ( f );
}


class Floor : public TaskBase
{
public:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	int numVertices = 0;
	virtual void OnCreate()
	{
		for (int j = -RANGE; j <= RANGE; ++j) {
			for (int i = -RANGE; i <= RANGE; ++i) {
				// 各四角形を2つの三角形で構成する
				// 四角形 (i, j) から (i+1, j+1)

				// 市松模様のパターンに合わせて色を決定
				// (i + j) が偶数なら黒、奇数なら白 (またはその逆)
				GLfloat current_color[4];
				if (((i + j) % 2) == 0) { // 例: 黒
					current_color[0] = 0.1f; current_color[1] = 0.1f; current_color[2] = 0.1f; current_color[3] = 1.0f;
				} else { // 例: 白
					current_color[0] = 1.0f; current_color[1] = 1.0f; current_color[2] = 1.0f; current_color[3] = 1.0f;
				}

				// 三角形1 (左下、右上、左上)
				// 1. (i, 0, j)
				vertices.push_back((GLfloat)i); vertices.push_back(0.0f); vertices.push_back((GLfloat)j);
				colors.push_back(current_color[0]); colors.push_back(current_color[1]); colors.push_back(current_color[2]); colors.push_back(current_color[3]);

				// 2. (i+1, 0, j+1)
				vertices.push_back((GLfloat)i+1.0f); vertices.push_back(0.0f); vertices.push_back((GLfloat)j+1.0f);
				colors.push_back(current_color[0]); colors.push_back(current_color[1]); colors.push_back(current_color[2]); colors.push_back(current_color[3]);

				// 3. (i, 0, j+1)
				vertices.push_back((GLfloat)i); vertices.push_back(0.0f); vertices.push_back((GLfloat)j+1.0f);
				colors.push_back(current_color[0]); colors.push_back(current_color[1]); colors.push_back(current_color[2]); colors.push_back(current_color[3]);

				// 三角形2 (左下、右下、右上)
				// 1. (i, 0, j)
				vertices.push_back((GLfloat)i); vertices.push_back(0.0f); vertices.push_back((GLfloat)j);
				colors.push_back(current_color[0]); colors.push_back(current_color[1]); colors.push_back(current_color[2]); colors.push_back(current_color[3]);

				// 2. (i+1, 0, j)
				vertices.push_back((GLfloat)i+1.0f); vertices.push_back(0.0f); vertices.push_back((GLfloat)j);
				colors.push_back(current_color[0]); colors.push_back(current_color[1]); colors.push_back(current_color[2]); colors.push_back(current_color[3]);

				// 3. (i+1, 0, j+1)
				vertices.push_back((GLfloat)i+1.0f); vertices.push_back(0.0f); vertices.push_back((GLfloat)j+1.0f);
				colors.push_back(current_color[0]); colors.push_back(current_color[1]); colors.push_back(current_color[2]); colors.push_back(current_color[3]);
			}
		}
		numVertices = vertices.size() / 3; // 各頂点はxyzの3要素

	}
	virtual void OnTick(float deltaTime) {}
	virtual void OnDraw()
	{
		// 頂点配列を有効にする
		glEnableClientState(GL_VERTEX_ARRAY);
		// 色配列を有効にする (glMaterialfv の代わり)
		glEnableClientState(GL_COLOR_ARRAY);

		// 頂点データと色データのポインタを指定
		// 第4引数はオフセットではなく、データへの直接ポインタです
		glVertexPointer(3, GL_FLOAT, 0, vertices.data());
		glColorPointer(4, GL_FLOAT, 0, colors.data());
		// --- ライティングの設定（元コードでglMaterialfvを使っているため） ---
		// AndroidのNdkBuild/CMakeLists.txtで -lGLESv1_common をリンクしていることを確認
		// glMaterialfv(GL_FRONT, GL_DIFFUSE, black); の代替
		// GL_COLOR_MATERIAL を有効にすることで、glColorPointer で指定された色が
		// GL_AMBIENT_AND_DIFFUSE のマテリアルカラーとして扱われます。
		glEnable(GL_COLOR_MATERIAL);
//		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		//glEnable(GL_LIGHTING); // ライティングを有効にする
		//glEnable(GL_LIGHT0);   // ライト0を有効にする

		// 法線を設定する（平面なので、法線は全てY軸方向 (0, 1, 0)）
		// ライティングを有効にする場合、法線は必須です。
		glNormal3f(0.0f, 1.0f, 0.0f);

		// 描画
		// GL_QUADSはGLESにはないため、GL_TRIANGLESを使用
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

		// 描画後、有効にした状態を無効にする
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisable(GL_COLOR_MATERIAL); // GL_COLOR_MATERIAL も無効にする
		//glDisable(GL_LIGHTING);       // ライティングを無効にする
	}
};

class Boids;
class Barrier;
class Master : public TaskBase
{
public:
	virtual void OnCreate() {}
	virtual void OnTick(float deltaTime) {}
	virtual void OnDraw()
	{

	}
	vec3 mCenter;
	std::vector<Boids*> mBoids;
	vec3	calcCenter(void);
	Boids* searchNear(Boids* target);
	std::vector<Barrier*> mBarrier;
};

class Barrier: public TaskBase
{
public:
	std::vector<GLfloat> diskVertices;
	std::vector<GLfloat> diskNormals;
	std::vector<GLfloat> diskColors;
	int numDiskVertices = 0;

	std::vector<GLfloat> sideVertices;
	std::vector<GLfloat> sideNormals;
	std::vector<GLfloat> sideColors;
	int numSideVertices = 0;

	void setupShapeData() {
		const int SEGMENTS = 12; // j <= 12 なので12セグメント
		const float STEP = 360.0f / SEGMENTS; // 30.0f * 12 = 360.0f

		// --- 円盤（底面）データ生成 ---
		// 中心点 (0.0f, pos.y, 0.0f) と円周上の点

		// 中心点
		diskVertices.push_back(0.0f); diskVertices.push_back(pos.y); diskVertices.push_back(0.0f);
		diskNormals.push_back(0.0f); diskNormals.push_back(1.0f); diskNormals.push_back(0.0f); // 法線はY軸上向き
		diskColors.push_back(0.5f); diskColors.push_back(0.5f); diskColors.push_back(0.5f); diskColors.push_back(1.0f);

		for (int j = 0; j <= SEGMENTS; ++j) {
			float angle_rad = glm::radians((float)j * STEP);
			float s = sinf(angle_rad);
			float c = cosf(angle_rad);

			diskVertices.push_back(s); diskVertices.push_back(pos.y); diskVertices.push_back(c);
			diskNormals.push_back(0.0f); diskNormals.push_back(1.0f); diskNormals.push_back(0.0f); // 法線はY軸上向き
			diskColors.push_back(0.5f); diskColors.push_back(0.5f); diskColors.push_back(0.5f); diskColors.push_back(1.0f);
		}
		numDiskVertices = diskVertices.size() / 3;

		// --- 側面データ生成 ---
		// GL_TRIANGLE_STRIP
		// 頂点ペア (円周上の上端、対応する下端) を連続して追加
		for (int j = 0; j <= SEGMENTS; ++j) {
			float angle_rad = glm::radians((float)j * STEP);
			float s = sinf(angle_rad);
			float c = cosf(angle_rad);

			// 頂点の上端 (s, pos.y, c)
			sideVertices.push_back(s); sideVertices.push_back(pos.y); sideVertices.push_back(c);
			sideNormals.push_back(s); sideNormals.push_back(0.0f); sideNormals.push_back(c); // 側面からの外向き法線
			sideColors.push_back(0.5f); sideColors.push_back(0.5f); sideColors.push_back(0.5f); sideColors.push_back(1.0f);

			// 頂点の下端 (s, 0.0f, c)
			sideVertices.push_back(s); sideVertices.push_back(0.0f); sideVertices.push_back(c);
			sideNormals.push_back(s); sideNormals.push_back(0.0f); sideNormals.push_back(c); // 側面からの外向き法線
			sideColors.push_back(0.5f); sideColors.push_back(0.5f); sideColors.push_back(0.5f); sideColors.push_back(1.0f);
		}
		numSideVertices = sideVertices.size() / 3;
	}
	virtual void OnCreate()
	{
		auto master = dynamic_cast<Master*>(parent);// parent��Master�ł���
		assert(master);
		if (!master)
		{
			return;
		}
		bool k = false;
		while (!k)
		{
			pos.x = (randu() - 0.5f) * (float)RANGE * 1.5f;
			pos.y = (randu() * 0.5f + 0.25f) * RANGE_HEIGHT;
			pos.z = (randu() - 0.5f) * (float)RANGE * 1.5f;
			k = true;
			for (auto b : master->mBarrier)
			{
				if (lengthSquare(pos, b->pos) < 8.0f * 8.0f)
				{
					k = false;
					break;
				}
			}
		}
		setupShapeData();
	}
	virtual void OnTick(float deltaTime) {}
	virtual void OnDraw()
	{
		glPushMatrix();

		// 元のglTranslatef(pos.x, 0.0f, pos.z); に相当
		glTranslatef(pos.x, 0.0f, pos.z);

		// glMaterialfv の代替: glColorMaterial を使用して頂点色をマテリアル色にマップ
		glEnable(GL_COLOR_MATERIAL);
//		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		// 共通の灰色を設定（各頂点にこの色を割り当てる）
		GLfloat gray_color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

		// --- 円盤（底面）の描画 ---
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY); // 色配列も有効に

		glVertexPointer(3, GL_FLOAT, 0, diskVertices.data());
		glNormalPointer(GL_FLOAT, 0, diskNormals.data());
		glColorPointer(4, GL_FLOAT, 0, diskColors.data()); // 色を渡す

		// ここでGL_TRIANGLE_FANとして描画
		glDrawArrays(GL_TRIANGLE_FAN, 0, numDiskVertices);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		// --- 側面の描画 ---
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY); // 色配列も有効に

		glVertexPointer(3, GL_FLOAT, 0, sideVertices.data());
		glNormalPointer(GL_FLOAT, 0, sideNormals.data());
		glColorPointer(4, GL_FLOAT, 0, sideColors.data()); // 色を渡す

		// ここでGL_TRIANGLE_STRIPとして描画
		glDrawArrays(GL_TRIANGLE_STRIP, 0, numSideVertices);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		// glMaterialfv の代替で有効にしたものを無効にする
		glDisable(GL_COLOR_MATERIAL);
	}
};

class Boids : public TaskBase {
public:
	Master *mMaster = nullptr;
	vec3 dir;
	vec3 fwd;
	float spd;

	virtual void OnCreate() {
		pos.x = (randu() - 0.5f) * (float) RANGE_INIT;
		pos.y = (randu() * 0.9f + 0.1f) * (float) RANGE_HEIGHT;
		pos.z = (randu() - 0.5f) * (float) RANGE_INIT;
		dir.x = randu() * 30.0f - 15.0f;
		dir.y = randu() * 360.0f;
		dir.z = 0.0f;
		spd = randu() * (SPEED_MAX - SPEED_MIN) + SPEED_MIN;
		fwd = calcForward(dir);
		fwd *= spd;
	}

	virtual void OnTick(float deltaTime) {
		vec3 d, o;
		float r;

		auto cen = mMaster->mCenter;
		cen.x *= -0.5f;
		cen.z *= -0.5f;

		o = glm::vec3(0.0f);

		// �Փ˔���
		d = pos;
		d += fwd;
		r = 15.0f * 15.0f;
		std::vector<Barrier *>::iterator j;
		for (j = mMaster->mBarrier.begin(); j != mMaster->mBarrier.end(); ++j) {
			o = d;
			o -= (*j)->pos;
			if (o.x * o.x + o.y * o.y + o.z * o.z < r) break;
			if (o.x * o.x + o.z * o.z < r) break;
		}
		o = glm::vec3(0.0f);
		if (j != mMaster->mBarrier.end()) {
			d = calcDirection(pos, (*j)->pos);
			o.y += step(180.0f + d.y - dir.y);
			o.x -= step(dir.x);
		} else {
			// ��������
			if (pos.y < RANGE_HEIGHT * 0.1f) {
				o.x -= ANGLE_STEP;
			} else if (pos.y > RANGE_HEIGHT) {
				o.x += ANGLE_STEP;
			} else {
				auto n = mMaster->searchNear(this);
				d = calcDirection(pos, n->pos);
				r = lengthSquare(pos, n->pos);
				if (r < DISTANCE_MIN * DISTANCE_MIN) {
					// �߂�����̂ŗ����
					spd -= cosf(glm::radians(d.y - dir.y)) * SPEED_STEP;
					o.x += step(180.0f + d.x - dir.x);
					o.y += step(180.0f + d.y - dir.y);
				} else if (r > DISTANCE_MAX * DISTANCE_MAX) {
					// ��������̂ŋ߂Â�
					spd += cosf(glm::radians(d.y - dir.y)) * SPEED_STEP;
					o.x += step(d.x - dir.x);
					o.y += step(d.y - dir.y);
				} else if (lengthSquare(pos, cen) > DISTANCE_THRESHOLD * DISTANCE_THRESHOLD) {
					// ��������̂ŋ߂Â�
					d = calcDirection(pos, cen);
					spd += cosf(glm::radians(d.y - dir.y)) * SPEED_STEP;
					o.x += step(d.x - dir.x);
					o.y += step(d.y - dir.y);
				} else if (fabs(angle(n->dir.y - dir.y)) > ANGLE_THRESHOLD) {
					// �߂��ƌ��������킹��
					o.y += step(n->dir.y - dir.y);
				} else if (fabs(angle(n->dir.x - dir.x)) > ANGLE_THRESHOLD) {
					// �߂��ƌ��������킹��
					o.x += step(n->dir.x - dir.x);
				}
			}
		}
		o.z = -o.y;
		dir += o;

		if (spd < SPEED_MIN) spd = SPEED_MIN;
		if (spd > SPEED_MAX) spd = SPEED_MAX;
		if (dir.x < ANGLE_MIN) dir.x = ANGLE_MIN;
		if (dir.x > ANGLE_MAX) dir.x = ANGLE_MAX;
		dir.y = angle(dir.y);
		dir.z *= 0.95f;
		fwd = calcForward(dir);
		fwd *= spd;
		pos += fwd;
	}

	virtual void OnDraw() {
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(dir.y, 0.0f, 1.0f, 0.0f);
		glRotatef(dir.x, 1.0f, 0.0f, 0.0f);
		glRotatef(dir.z, 0.0f, 0.0f, 1.0f);
		glScalef(1.0f, 1.0f, 1.0f);

		// --- 側面の描画 ---
		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices.data());

		// ここでGL_TRIANGLE_STRIPとして描画
		glDrawArrays(GL_LINE_STRIP, 0, numVertices);

		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
	}

	static inline std::vector<vec3> vertices;
	static inline int numVertices = 0;

	static void SetupShapeData() {

		// --- 円盤（底面）データ生成 ---
		// 中心点 (0.0f, pos.y, 0.0f) と円周上の点

		vertices.push_back(vec3(0.0f, 0.0f, -0.2f));
		vertices.push_back(vec3(0.1f, 0.0f, -0.2f));
		vertices.push_back(vec3(0.0f, 0.0f, 0.2f));
		vertices.push_back(vec3(-0.1f, 0.0f, -0.2f));
		vertices.push_back(vec3(0.0f, 0.0f, -0.2f));
		vertices.push_back(vec3(0.0f, 0.0f, 0.2f));
		vertices.push_back(vec3(0.0f, -0.1f, -0.2f));
		vertices.push_back(vec3(0.0f, 0.0f, -0.2f));
		numVertices = vertices.size();
	}
};

vec3	Master::calcCenter(void)
{
	mCenter = vec3(0.0f);
	if (mBoids.size() > 0)
	{
		for (auto i : mBoids)
		{
			mCenter += i->pos;
		}
		mCenter /= (float)mBoids.size();
	}
	return mCenter;
}

Boids* Master::searchNear(Boids* target)
{
	Boids* s = target;
	float rs = FLT_MAX;
	for (auto b : mBoids)
	{
		if (b == target)
		{
			continue;
		}
		auto r = lengthSquare(b->pos, target->pos);
		if (r < rs)
		{
			s = b;
			rs = r;
		}
	}
	return s;
}

class Camera : public TaskBase
{
public:
	float mRot = 0.0f;
	virtual void OnTick(float deltaTime)
	{
		mRot += 0.05f;
		if (mRot >= 360.0f)	mRot -= 360.0f;
	}
	virtual void OnDraw()
	{
		float aspect = (float)WndWidth / (float)WndHeight;

		glViewport(0, 0, WndWidth, WndHeight);

		auto proj = glm::perspective(
			glm::radians(60.0f),
			aspect,
			0.1f,
			32767.0f);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(proj));

		auto ang = glm::radians(mRot);
		auto view = glm::lookAt(
			glm::vec3(sinf(ang) * 15.0f, 10.0f, cosf(ang) * 15.0f),
			(mMaster?mMaster->mCenter:vec3(0.0f)),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(view));
	}

	Master* mMaster = nullptr;
};

class Enviroment : public TaskBase
{
public:
	Master* mMaster = nullptr;
	virtual void OnTick(float deltaTime) {}
	virtual void OnDraw()
	{
		glEnable(GL_LIGHT0);		// ���C�g�O��L���ɂ���
		glEnable(GL_LIGHTING);		// ���C�e�B���O��L���ɂ���
		glEnable(GL_DEPTH_TEST);	// �[�x�e�X�g��L���ɂ���
		glShadeModel(GL_SMOOTH);	// �X���[�X�V�F�[�f�B���O��L���ɂ���
		glEnable(GL_BLEND);			// ��������L���ɂ���
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// ���C�g�O�̈ʒu
		vec3 cen(10.0f, 10.0f, 10.0f);
		if (mMaster)
		{
			cen = mMaster->calcCenter();
		}
		float	light[] = { cen.x, cen.y * 2, cen.z, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light);

		// �ގ��̃A���r�G���g�J���[
		float	ambient[] = { .1f, .1f, .1f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

		// �ގ��̔��ˌ�
		float	spec[] = { .9f, .9f, .9f, 1.0f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);

		// �ގ��̌���
		float	glos[] = { 128.0f };
		glMaterialfv(GL_FRONT, GL_SHININESS, glos);
	}
};



TaskSystem ts;

void Boids_Init()
{
	auto cam = ts.CreateTask<Camera>(nullptr, "Camera",0,0);
	auto env = ts.CreateTask<Enviroment>(nullptr, "Enviroment",0,1);
	auto stage = ts.CreateTask<Floor>(nullptr, "Floor",0,2);
	auto master = ts.CreateTask<Master>(nullptr, "Master",0,3);
	cam->mMaster = master;
	env->mMaster = master;
	Boids::SetupShapeData();
	for (auto i = 0; i < NUMBER; i++) {
		auto b = ts.CreateTask<Boids>(master, "Boids",4+i);
		b->mMaster = master;
		master->mBoids.push_back(b);
	}
	for (auto i = 0; i < OBJECT; i++) {
		auto b = ts.CreateTask<Barrier>(master, "Barrier",0,1000+i);
		master->mBarrier.push_back(b);
	}
}

void Boids_Reshape( int w, int h )
{
	WndWidth = w;
	WndHeight = h;
}

 void	Boids_FrameMove( void )
{
	ts.Tick(1.0f);
}

 void	Boids_FrameDraw( void )
{
	glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ts.Draw();
};

