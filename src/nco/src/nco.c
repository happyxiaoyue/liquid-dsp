//
// Numerically-controlled oscillator
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "nco.h"

void nco_init(nco _nco)
{
    _nco.theta = 0.0f;
    _nco.d_theta = 0.0f;
}

void nco_set_phase(nco _nco, float _phi)
{
    _nco.theta = _phi;
    nco_constrain_phase(_nco);
}

void nco_adjust_phase(nco _nco, float _dphi)
{
    _nco.theta += _dphi;
    nco_constrain_phase(_nco);
}

void nco_step(nco _nco)
{
    _nco.theta += _nco.d_theta;
    nco_constrain_phase(_nco);
}

void nco_constrain_phase(nco _nco)
{
    if (_nco.theta > M_PI)
        _nco.theta -= 2.0f*M_PI;
    else if (_nco.theta < -M_PI)
        _nco.theta += 2.0f*M_PI;
}

// Rotate input vector array up by NCO angle, \f$\vec{y} = \vec{x}e^{j\theta}\f$
void nco_mix_block_up(
    nco _nco,
    complex float *_x,
    unsigned int _N,
    complex float *_y)
{
    unsigned int i;

#if 0
    for (i=0; i<_N; i++) {
        nco_mix_up(_nco, _xi[i], _xq[i], &_yi[i], &_yq[i]);
        nco_step(_nco);
    }
#else
    float theta =   _nco.theta;
    float d_theta = _nco.theta;
    for (i=0; i<_N; i++) {
        //sigprocc_rotate(_xi[i], _xq[i], theta, &_yi[i], &_yq[i]);
        _y[i] = _x[i] * cexpf(I*theta);
        
        // nco_step(_nco);
        theta += d_theta;
    }

    // nco_constrain_phase(_nco);
    while (theta > M_PI)
        theta -= 2*M_PI;
    while (theta < -M_PI)
        theta += 2*M_PI;

    nco_set_phase(_nco, theta);
#endif
}

