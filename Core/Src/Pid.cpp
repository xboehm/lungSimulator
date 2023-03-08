#include "Pid.hpp"

PID::PID(float kp, float ki, float kd, float tau, 
int limMin, int limMax, float limMinInt, float limMaxInt, float t)
	: m_Kp(kp), m_Ki(ki), m_Kd(kd), m_tau(tau), m_limMin(limMin), m_limMax(limMax),
		m_limMinInt(limMinInt), m_limMaxInt(limMaxInt), m_T(t) {}

int PID::update(float setpoint, float measurement) {

	/*
	* Error signal
	*/
    m_error = setpoint - measurement;


	/*
	* Proportional
	*/
    float proportional = m_Kp * m_error;


	/*
	* Integral
	*/
    m_integrator = m_integrator + 0.5f * m_Ki * m_T * (m_error + m_prevError);

	/* Anti-wind-up via integrator clamping */
    if (m_integrator > m_limMaxInt) {

        m_integrator = m_limMaxInt;

    } else if (m_integrator < m_limMinInt) {

        m_integrator = m_limMinInt;

    }


	/*
	* Derivative (band-limited differentiator)
	*/
		
    m_differentiator = -(2.0f * m_Kd * (measurement - m_prevMeasurement)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * m_tau - m_T) * m_differentiator)
                        / (2.0f * m_tau + m_T);


	/*
	* Compute output and apply limits
	*/
    float out_f = proportional + m_integrator + m_differentiator;
    int out_i {static_cast<int>(out_f)};
    if (out_i > m_limMax) {
        out_i = m_limMax;
    }
    else if (out_i < m_limMin) {
        out_i = m_limMin;
    }

	/* Store error and measurement for later use */
    m_prevError       = m_error;
    m_prevMeasurement = measurement;

	/* Return controller output */
    return out_i;

}
