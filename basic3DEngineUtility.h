struct vector3I {
  int x, y, z;
  vector3I(int _x = 0, int _y = 0, int _z = 0) {
    x = _x;
    y = _y;
    z = _z;
  }
  vector3I multiply(int _val) {
    vector3I result;
    x = x * _val;
    y = y * _val;
    z = z * _val;
    result = vector3I(x, y, z);
    return result;
  }
};
struct vector3F {
  double x, y, z;
  vector3F(double _x = 0, double _y = 0, double _z = 0) {
    x = _x;
    y = _y;
    z = _z;
  }
  vector3I ToInt() {
    vector3I result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
  }
};
struct vector2I {
  int x, y;
  vector2I(int _x = 0, int _y = 0) {
    x = _x;
    y = _y;
  }
};

struct angle {
private:
  unsigned int val = 0;
public:
  angle(unsigned int _val = 0) {
    val = _val;
    if (val > 360) {
      val = 0;
    }
  }
  void setAngle(unsigned int _val) {
    val = _val;
    if (val > 360) {
      val = 0;
    }
  }
  unsigned int getAngle() {
    if (val > 360) {
      val = 0;
    }
    return val;
  }
};