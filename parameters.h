#ifndef PARAMETERS_H
#define PARAMETERS_H


class Parameters
{
    bool _wireframe = false; // To display the wireframe or not
    bool _axis = true; // To display the axis or not
public:
    Parameters(bool wireframe, bool axis)  : _wireframe(wireframe), _axis(axis) {}
    Parameters() : _wireframe(false), _axis(true) {}

    bool wireframe() const { return _wireframe; }
    bool axis() const { return _axis; }

    void setWireframe(bool wireframe) { _wireframe = wireframe; }
    void setAxis(bool axis) { _axis = axis; }
};

#endif // PARAMETERS_H
