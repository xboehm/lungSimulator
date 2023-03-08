#pragma once

class PID {
private:
	/* Controller gains */
	float m_Kp;
	float m_Ki;
	float m_Kd;

	/* Derivative low-pass filter time constant */
	float m_tau;

	/* Output limits */
	int m_limMin;
	int m_limMax;
	
	/* Integrator limits */
	float m_limMinInt;
	float m_limMaxInt;

	/* Sample time (in seconds) */
	float m_T;

	/* Controller "memory" */
	float m_integrator {0.0f};
	float m_prevError {0.0f};			/* Required for integrator */
	float m_differentiator {0.0f};
	float m_prevMeasurement {0.0f};		/* Required for differentiator */

	/* Controller signals */
	
	float m_error {0.0f};
	
public:
	PID(float m_Kp, float m_Ki, float m_Kd, float m_tau, int m_limMin, int m_limMax, float m_limMinInt, float m_limMaxInt, float m_T);
	int update(float setpoint, float measurement);
};
