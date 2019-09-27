#pragma once
#include <iostream>
#include <string>
#include <math.h>

#pragma region Vector2D
class Vector2D
{
public:

	Vector2D();
	Vector2D(float x, float y);

	~Vector2D();

	void Set(float x, float y);
	void SetX(float x);
	void SetY(float y);
	inline float GetX() { return cord[0]; }
	inline float GetY() { return cord[1]; }
	inline void Print() const { printf("%0.1f : %0.1f\n", cord[0], cord[1]); }
	float Norm();

	float operator*(Vector2D rhs);
	void operator*(float rhs);
	Vector2D operator+(Vector2D& rhs);
	Vector2D operator-(Vector2D& rhs);
	void operator=(Vector2D& rhs);
	bool operator==(Vector2D& rhs);
	bool operator<(Vector2D& rhs);
	bool operator<=(Vector2D& rhs);
	bool operator>(Vector2D& rhs);
	bool operator>=(Vector2D& rhs);
	bool operator!=(Vector2D& rhs);
	float& operator[] (const int x);
	float operator[] (const int x) const;


private:

	float cord[2];

};
#pragma endregion

#pragma region Matrix2D
class Matrix2D
{
public:

	//Konstructorer
	Matrix2D();												// skapar en identitetsmatris
	Matrix2D(float x1, float y1, float x2, float y2);		// skapar en matris med fyra v�rden
	Matrix2D(Vector2D x, Vector2D y);							// skapar en matris med tv� vectorer (sparas p� samma s�tt..)
	Matrix2D(const Matrix2D& a);							// skapar en kopia av matrisen.
	Matrix2D(float degrees);

	~Matrix2D();

	//Funktioner
	void Set(float x1, float y1, float x2, float y2);		// s�tter v�rden med fyra v�rden
	void Set(Vector2D x, Vector2D y);							// s�tter v�rden med tv� vectorer
	void Set(float degrees);
	void Rot(float degrees);
	void Transponat();

	inline float GetX1() const { return matrix[0][0]; };	// return row 1 coloum 1
	inline float GetY1() const { return matrix[0][1]; };	// return row 1 coloum 2
	inline float GetX2() const { return matrix[1][0]; };	// return row 2 coloum 1
	inline float GetY2() const { return matrix[1][1]; };	// return row 2 coloum 2
	inline void Print() const { printf("\n Matrix = %0.1f : %0.1f \n          %0.1f : %0.1f\n", matrix[0][0], matrix[0][1], matrix[1][0], matrix[1][1]); }

	//Operatorer
	void operator=(Matrix2D& rhs);							//kolla (matrix = matrix) = kopia med sammam v�rden. 
	void operator*(float rhs);								//kolla (matrix * 4)
	Vector2D operator*(Vector2D rhs);							//kolla (matrix * vector)
	Matrix2D operator*(Matrix2D rhs);

private:

	float matrix[2][2];

};
#pragma endregion

#pragma region Matrix3D
class Matrix3D
{
public:

	//Konstructorer
	Matrix3D();
	Matrix3D(float x1, float y1, float x2, float y2);
	Matrix3D(Vector2D x, Vector2D y);
	Matrix3D(const Matrix2D& a);
	Matrix3D(float degrees);

	~Matrix3D();

	//Funktioner
	void Set(float x1, float y1, float x2, float y2);		// s�tter v�rden med fyra v�rden
	void Set(Vector2D x, Vector2D y);							// s�tter v�rden med tv� vectorer
	void Set(float degrees);
	void SetSize(float sizeX, float sizeY);
	void Rot(float degrees);
	void Transponat();

	void SetPosition(Vector2D v);
	Vector2D GetPosition();
	void TranslationMatrix(Vector2D v);

	inline float GetX1()		const { return matrix[0][0]; };	// return row 1 coloum 1
	inline float GetY1()		const { return matrix[0][1]; };	// return row 1 coloum 2
	inline float GetZ1()		const { return matrix[0][2]; };	// return row 1 coloum 3
	inline float GetX2()		const { return matrix[1][0]; };	// return row 2 coloum 1
	inline float GetY2()		const { return matrix[1][1]; };	// return row 2 coloum 2
	inline float GetZ2()		const { return matrix[1][2]; };	// return row 2 coloum 3
	inline float GetX3()		const { return matrix[2][0]; };	// return row 3 coloum 1
	inline float GetY3()		const { return matrix[2][1]; };	// return row 3 coloum 2
	inline float GetZ3()		const { return matrix[2][2]; };	// return row 3 coloum 3
	inline float GetSizeX()		const { return matrix[2][0]; };
	inline float GetSizeY()		const { return matrix[2][1]; };
	inline float GetRadius()	const { return matrix[2][2]; };
	inline void Print()			const { printf("\n Matrix = %0.1f : %0.1f \n          %0.1f : %0.1f \n Position X = %0.1f \n          Y = %0.1f", matrix[0][0], matrix[0][1], matrix[1][0], matrix[1][1], matrix[0][2], matrix[1][2]); }
	//Operatorer
	Vector2D operator* (Vector2D& rhs);

private:

	float matrix[3][3];

};
#pragma endregion

#pragma region Vector4D
class Vector4D
{
public:

	explicit Vector4D(float x = 0, float y = 0, float z = 0, float w = 1);

	~Vector4D();

	void Set(float x, float y, float z, float w);

	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetW(float w);

	inline float GetX() { return cord[0]; }
	inline float GetY() { return cord[1]; }
	inline float GetZ() { return cord[2]; }
	inline float GetW() { return cord[3]; }
	inline float* GetPointer() { return cord; }

	inline void Print() const { printf("%0.1f : %0.1f : %0.1f : %0.1f\n", cord[0], cord[1], cord[2], cord[3]); }
	Vector4D Normalize();
	float Length();
	static Vector4D cross(Vector4D first, Vector4D last);

	Vector4D operator*(float rhs);		//Scaling
	float operator*(Vector4D rhs);		//Dot Multi
	Vector4D operator&&(Vector4D rhs);	//Cross Multi  "&&"  may change
	Vector4D operator+(Vector4D& rhs);	//Addition
	Vector4D operator-(Vector4D& rhs);	//Subtraction
	void operator+=(Vector4D rhs);
	void operator-=(Vector4D rhs);
	float& operator[] (const int x);
	float operator[] (const int x) const;

private:
	float cord[4];
};
#pragma endregion

#pragma region Matrix4D
class Matrix4D {

public:

    /// Konstruktorer

    Matrix4D(float x1 = 1, float y1 = 0, float z1 = 0, float w1 = 0, float x2 = 0, float y2 = 1, float z2 = 0,
             float w2 = 0, float x3 = 0, float y3 = 0, float z3 = 1, float w3 = 0, float x4 = 0, float y4 = 0,
             float z4 = 0, float w4 = 1);

    Matrix4D(Vector2D a, Vector2D b, Vector2D c, Vector2D d, Vector2D e, Vector2D f, Vector2D g, Vector2D h);

    Matrix4D(const Matrix2D &a, const Matrix2D &b, const Matrix2D &c, const Matrix2D &d);

    Matrix4D(const Matrix3D &a);

    Matrix4D(Vector4D a, float degrees);

    Matrix4D(Vector4D R, Vector4D U, Vector4D D);

    Matrix4D(Vector4D P);

    ~Matrix4D();

    /// Functions
    void
    Set(float x1 = 0, float y1 = 0, float z1 = 0, float w1 = 0, float x2 = 0, float y2 = 0, float z2 = 0, float w2 = 0,
        float x3 = 0, float y3 = 0, float z3 = 0, float w3 = 0, float x4 = 0, float y4 = 0, float z4 = 0, float w4 = 0);

    void Set(Vector2D a, Vector2D b, Vector2D c, Vector2D d, Vector2D e, Vector2D f, Vector2D g, Vector2D h);

    void Set(const Matrix2D &a, const Matrix2D &b, const Matrix2D &c, const Matrix2D &d);

    void SetI();

    inline void SetX1(float a) { matrix[0][0] = a; };

    inline void SetY1(float a) { matrix[0][1] = a; };

    inline void SetZ1(float a) { matrix[0][2] = a; };

    inline void SetW1(float a) { matrix[0][3] = a; };

    inline void SetX2(float a) { matrix[1][0] = a; };

    inline void SetY2(float a) { matrix[1][1] = a; };

    inline void SetZ2(float a) { matrix[1][2] = a; };

    inline void SetW2(float a) { matrix[1][3] = a; };

    inline void SetX3(float a) { matrix[2][0] = a; };

    inline void SetY3(float a) { matrix[2][1] = a; };

    inline void SetZ3(float a) { matrix[2][2] = a; };

    inline void SetW3(float a) { matrix[2][3] = a; };

    inline void SetX4(float a) { matrix[3][0] = a; };

    inline void SetY4(float a) { matrix[3][1] = a; };

    inline void SetZ4(float a) { matrix[3][2] = a; };

    inline void SetW4(float a) { matrix[3][3] = a; };

    inline void TransposeThis() {
        Matrix4D temp = Matrix4D();
        temp = *this;

        this->matrix[0][1] = temp.matrix[1][0]; // 1
        this->matrix[1][0] = temp.matrix[0][1]; // 2

        this->matrix[0][2] = temp.matrix[2][0]; // 3
        this->matrix[2][0] = temp.matrix[0][2]; // 4

        this->matrix[1][2] = temp.matrix[2][1]; // 5
        this->matrix[2][1] = temp.matrix[1][2]; // 6

        this->matrix[0][3] = temp.matrix[3][0]; // 7
        this->matrix[3][0] = temp.matrix[0][3]; // 8

        this->matrix[1][3] = temp.matrix[3][1]; // 9
        this->matrix[3][1] = temp.matrix[1][3]; // 10

        this->matrix[2][3] = temp.matrix[3][2]; // 11
        this->matrix[3][2] = temp.matrix[2][3]; // 12

        temp.~Matrix4D();
    };    // Transposes the activ Matrix, does not create a new matrix.
    void InversThis();        // Inverses the activ Matrix, does not create a new matrix.
    void Print();            // Prints a easy readout.
    void GetArr();            // Converts into arr[16].
    float *GetPointer();

    static Matrix4D lookAt(Vector4D cameraPosition, Vector4D cameraDirection, Vector4D cameraUp);

    void MakeToMatrix();    // Used in InvertThis to make arr to matrix
    inline Vector4D GetPositionVector()
    {
        this->GetArr();
        return Vector4D(this->arr[3], this->arr[7] ,this->arr[11] ,1);
    }
    inline void GetPositionMatrix(Vector4D vec)
    {
        this->Set(1, 0, 0, vec.GetX(),
                  0, 1, 0, vec.GetY(),
                  0, 0, 1, vec.GetZ(),
                  0, 0, 0, 1);
    };
	inline void GetScaleMatrix(Vector4D vec)
	{
        this->Set(vec.GetX(),0,0,0,
                  0,vec.GetY(),0,0,
                  0,0,vec.GetZ(),0,
                  0,0,     0,1);
	};
	inline void GetRotationFromQuaternian(Vector4D vec)
    {
        float xx = vec[0] * vec[0];
        float xy = vec[0] * vec[1];
        float xz = vec[0] * vec[2];
        float xw = vec[0] * vec[3];

        float yy = vec[1] * vec[1];
        float yz = vec[1] * vec[2];
        float yw = vec[1] * vec[3];

        float zz = vec[2] * vec[2];
        float zw = vec[2] * vec[3];

        this->Set(1-2*(yy+zz),   2*(xy-zw),   2*(xz+yw), 0,
                    2*(xy+zw), 1-2*(xx+zz),   2*(yz-xw), 0,
                    2*(xz-yw),   2*(yz+xw), 1-2*(xx+yy), 0,
                            0,           0,           0, 1);
    };


	static Matrix4D RotVec(Vector4D Vec, float degrees);			// Makes a rotation matrix around a vector.
	static Matrix4D RotX(float degrees);					// Makes a rotation matrix in the X-axis
	static Matrix4D RotY(float degrees);					// Makes a rotation matrix in the Y-axis
	static Matrix4D RotZ(float degrees);					// Makes a rotation matrix in the Z-axis

	Matrix4D operator/(float rhs);
	Matrix4D operator*(float rhs);
	Vector4D operator*(Vector4D rhs);
	Matrix4D operator*(Matrix4D rhs);
	void operator+=(Matrix4D rhs);

private:
	float matrix[4][4];
	float arr[16];
};
#pragma endregion