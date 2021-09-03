/** @file */
#pragma once
//	------------------------------------------------------
typedef struct {
	float A = 0.08; // (m) demie voie
	float D  = 0.08; // (m) distance centre roues gravité
	float R  = 0.03; // (m) rayon des roues
	float Mr  = 0.400; // (kg) masse rover sans moteurs & roues
	float Mw  = 0.100; // (kg) masse un moteur & roue
	float Irz  = 0.0003; // (kg.m²) moment d'inertie %z & centre Gravité rover sans moteurs & roues
	float Iwy  = 0.0001; // (kg.m²) moment d'inertie %y rover & axe roue 1 roue un moteur & roue 
	float Iwz  = 0.0002; // (kg.m²) moment d'inertie %z &  verticale diamètre roue un moteur & roue 
	float Rrx  = 400.0; // (N : kg.m.s⁻²) force de réaction longitudinale droite
	float Rlx  = 400.0; // (N : kg.m.s⁻²) force de réaction longitudinale gauche
	float Rry  = 400.0; // (N : kg.m.s⁻²) force de réaction latérale droite
	float Rly  = 400.0; // (N : kg.m.s⁻²) force de réaction latérale gauche
	float Frx  = 400.0; // (N : kg.m.s⁻²) force de traction longitudinale droite
	float Flx  = 400.0; // (N : kg.m.s⁻²) force de traction longitudinale gauche
	float Fry  = 400.0; // (N : kg.m.s⁻²) force de traction latérale droite
	float Fly  = 400.0; // (N : kg.m.s⁻²) force de traction latérale gauche
	float Cr  = 400.0; // (N.m : kg.m².s⁻²) couple moteur roue droite
	float Cl  = 400.0; // (N.m : kg.m².s⁻²) couple moteur roue  gauche
	
} s_roverModel;
/*
𝑀(𝑞)𝑞̈ + 𝐶(𝑞, 𝑞̇ ) = 𝐵(𝑞)𝜏 + 𝐹(𝑞̇ ) + Λ 𝑇 (𝑞)𝜆
	𝑀(𝑞) est une matrice d’inertie symétrique définie positive. 
	𝐶(𝑞, 𝑞̇ ) est la matrice des forces centrifuge et de Coriolis. 
	𝐵(𝑞) est une matrice constante non singulière. 
	𝐹(𝑞̇ ) la matrice de frottement de la surface. 
	Λ 𝑇 (𝑞) est la matrice des contraintes non-holonomes. 
	𝜆 le vecteur des multiplicateurs de Lagrange.
	
	
𝑚 𝑟 𝑥̈ 𝐺 = −(𝑅 1 + 𝑅 2 ) sin(𝜑) + (𝑅 3 + 𝑅 4 ) cos (𝜑) 	(1.35a)
𝑚 𝑟 𝑦̈ 𝐺 = (𝑅 1 + 𝑅 2 ) cos(φ) + (𝑅 3 + 𝑅 4 ) sin (𝜑) 	(1.35b)
(𝐼 𝑟𝑧 + 2 𝐼 𝑤𝑧 ) 𝜙̈ = 𝑑 (𝑅 1 + 𝑅 2 ) + 𝑎 (𝑅 3 − 𝑅 4 ) 		(1.35c)
𝑚 𝑤 𝜂̈ 𝑟 + 𝑚 𝑤 𝜙̇ 𝜌̇ 𝑟 = 𝑓 𝑙𝑎𝑡𝑟 − 𝑅 1 						(1.35d)
𝑚 𝑤 𝜂̈ 𝑙 + 𝑚 𝑤 𝜙̇ 𝜌̇ 𝑙 = 𝑓 𝑙𝑎𝑡𝑙 − 𝑅 2 						(1.35e)
𝑚 𝑤 𝜌̈ 𝑟 − 𝑚 𝑤 𝜙̇ 𝜂̇ 𝑟 = 𝑓 𝑙𝑜𝑛𝑔𝑟 − 𝑅 3 						(1.35f)
𝑚 𝑤 𝜌̈ 𝑙 − 𝑚 𝑤 𝜙̇ 𝜂̇ 𝑙 = 𝑓 𝑙𝑜𝑛𝑔𝑙 − 𝑅 4 						(1.35g)
𝐼 𝑤𝑦 𝜃̈ 𝑟 = 𝜏 𝑟 − 𝑓 𝑙𝑜𝑛𝑔𝑟 𝑟 								(1.35h)
𝐼 𝑤𝑦 𝜃̈ 𝑙 = 𝜏 𝑙 − 𝑓 𝑙𝑜𝑛𝑔𝑙 𝑟									(1.35i)
	
*/
s_roverModel rovMod;


float _1on2A  = 1/2.0/A;
float _1on4A  = _1on2A/2.0;
float sinPhi  = sin(Phi);
float cosPhi  = cos(Phi);
/*
q est le vecteur des coordonnées généralises Vect9 (X,Y,Phi,glatr,glatl,glongr,glongl,Teta right, Teta left)
*/
Matrix Sq=	Matrix(9,5);

Sq[0][0]=-sinPhi;
Sq[0][1]=(A*cosPhi+D*sinPhi)*_1on2A;
Sq[0][2]=(A*cosPhi-D*sinPhi)*_1on2A;

Sq[1][0]=+cosPhi;
Sq[1][1]=(A*sinPhi-D*cosPhi)*_1on2A;
Sq[1][2]=(A*sinPhi+D*cosPhi)*_1on2A;

Sq[2][1]=+_1on2A;
Sq[2][2]=-_1on2A;

Sq[3][0]=1;
Sq[4][0]=1;
Sq[5][1]=1;
Sq[6][2]=1;
Sq[7][3]=1;
Sq[8][4]=1;

Matrix Mq=	Matrix(9,9);
Mq[0][0]=rovMod.Mr;
Mq[1][1]=rovMod.Mr;
Mq[2][2]=rovMod.Irz+2*rovMod.Iwz;
Mq[3][3]=rovMod.Mw;
Mq[4][4]=rovMod.Mw;
Mq[5][5]=rovMod.Mw;
Mq[6][6]=rovMod.Mw;
Mq[7][7]=rovMod.Iwy
Mq[8][8]=rovMod.Iwz;

Matrix q=	Matrix(9,1)={Xg,Yg,Phi,Nur,Nul,Ror,Rol,Ter,Tel};

//	------------------------------------------------------

typedef std::array<float, 3> t_vct3F;
typedef std::array<float, 4> t_vct4F;
typedef std::array<t_vct4F, 4> t_mat4F;
/*
 typedef float t_vct3F[3];
 typedef float t_vct4F[4];
 typedef float t_mat4F[4][4];
 */
/**
 *
 * s_kineKeyframe KF; //trajectory Key frame (repère clé)
 */
typedef struct {
	float qualityMin = +1.0e+4;
	float qualityMax = -1.0e+4;
	t_vct4F DXYZ = { { (float)0.0, (float)0.0, (float)0.0, (float)1.0 } }; // from linear accelerations
	t_mat4F MATEXT = { {
		{ { (float)1.0, (float)0.0, (float)0.0, (float)0.0 } },
		{ { (float)0.0, (float)1.0, (float)0.0, (float)0.0 } },
		{ { (float)0.0, (float)0.0, (float)1.0, (float)0.0 } },
		{ { (float)0.0, (float)0.0, (float)0.0, (float)1.0 } }
	} };
	char MSG[36] = { '\0' }; // for ring queue messaging
} s_kineKeyframe;
String S_Kine(const uint64_t MTS, const uint8_t IDX, const char CAR,
	const t_vct3F &ACC, const t_vct3F &GYR, const t_vct3F &MAG,
	const t_vct3F &SPD, const char *MSG);
///////////////////////////////////////////////////////////////////////
class Kine_1: public Core {
private:
	s_kineCTX &_kineCTX;
	c_stat_3F stat_As, /*stat_Vs,*/ stat_Ds;
	c_stat_3F stat_An, stat_Gn,stat_Grav, stat_Mn;
	/**
	 * compute s_kineKeyframe Position & external Attitude matrix (EXTMAT like)
	 * Cf. https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	 * Cf. https://fr.wikipedia.org/wiki/Quaternions_et_rotation_dans_l%27espace
	 */
	void calc_KEYFRAME();
	void calc_ATTITUDE(const float LMD);
	void init_statistics();

public:
	uint64_t curMicroTS=0;/**< microseconds time current kinematic's update (@ref updateRoverKine) */
	uint64_t deltaMicroTS=0;/**< microseconds time since the previous kinematic's update (@ref updateRoverKine) */
	s_kineFusensor Fusensor;// µTS == prev MicroTS after updateRoverKine !!!
	s_kineAttitude Attitude;// µTS == prev MicroTS after updateRoverKine !!!
	s_kineKeyframe Keyframe;// µTS == prev MicroTS after updateRoverKine !!!
	Kine_1(s_kineCTX *KINECTX, const String NAME, int8_t *DBGMAX);
	void Predict();
	void Clear();
	/**
	 * @fn void updateRoverKine(uint8_t &TELEMETRY, const bool TELEM, bool &CLROPNGL, const bool OPNGL,	const char *NL0, const char *NLN, const float LMD)
	 * @brief updates the attitude, acceleration,velocity, position... and the frame matrix
	 *
	 *
	 * @param TELEMETRY returns 0 & stops CSV telemetry if UDP error when sending else unchanged
	 * @param TELEM activates CSV telemetry if true
	 * @param CLROPNGL if true, returns false after clearing openGL
	 * @param OPNGL activates openGL serial telemetry
	 * @param NL0 prefix for the first line on UDP dashboard (permits to interlace 2 Kine timings on dashboard)
	 * @param NLN prefix for the next lines on the UDP dashboard (permits to interlace 2 Kine timings on dashboard)
	 * @param LMD
	 */
	void updateRoverKine( const float LMD = 0.0);
	void dump_FUSENSOR(const size_t SIZ, int16_t &CNT, char *BUF,
		const char *NL0 = "\n", const char *NLN = "\n");
	void dump_ATTITUDE(const size_t SIZ, int16_t &CNT, char *BUF,
		const char *NL0 = "\n", const char *NLN = "\n");
	void dump_KFMATRIX(const size_t SIZ, int16_t &CNT, char *BUF,
		const char *NL0 = "\n", const char *NLN = "\n");
	void telemetry_KEYFRAME(const size_t SIZ, int16_t &CNT, char *BUF)const;
	void telemetry_FUSENSOR(const size_t SIZ, int16_t &CNT, char *BUF)const;
	void telemetry_ATTITUDE(const size_t SIZ, int16_t &CNT, char *BUF)const;
};
inline void Kine_1::init_statistics(){
	stat_As.init();
//	stat_Vs.init();
	stat_Ds.init();
	stat_An.init(-1);
	stat_Gn.init();
	stat_Mn.init();
	stat_Grav.init();
}
///////////////////////////////////////////////////////////////////////
