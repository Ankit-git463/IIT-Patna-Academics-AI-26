import numpy as np

def calculate_power_flow(system_base_mva=100):
    # System constants
    BUS_TYPE_SLACK = 1
    BUS_TYPE_PQ = 2
    BUS_TYPE_PV = 3

    # Transmission line parameters [from_bus, to_bus, resistance, reactance, susceptance]
    transmission_lines = np.array([
        [1, 2, 0.02, 0.04, 0],
        [1, 3, 0.01, 0.03, 0],
        [2, 3, 0.0125, 0.025, 0]
    ])

    # Bus parameters [bus_number, type, P_gen, Q_gen, P_load, Q_load, voltage_magnitude, voltage_angle, Q_min, Q_max]
    bus_parameters = np.array([
        [1, 1, 0, 0, 0, 0, 1.05, 0, 0, 0],
        [2, 2, 0, 0, 4, 2.5, 1, 0, 0, 0],
        [3, 3, 2, 0, 0, 0, 1.04, 0, 0, 0]
    ])

    # Extract line parameters
    line_resistance = transmission_lines[:, 2]
    line_reactance = transmission_lines[:, 3]
    line_susceptance = 1j * transmission_lines[:, 4]
    line_impedance = line_resistance + 1j * line_reactance
    line_admittance = 1 / line_impedance

    # System dimensions
    num_transmission_lines = len(transmission_lines)
    num_buses = int(max(transmission_lines[:, :2].max(), transmission_lines[:, 1:2].max()))
    num_generators = np.sum(bus_parameters[:, 1] == 2)

    # Form admittance matrix (Y-bus)
    admittance_matrix = np.zeros((num_buses, num_buses), dtype=complex)
    for idx in range(num_transmission_lines):
        from_bus = int(transmission_lines[idx, 0] - 1)
        to_bus = int(transmission_lines[idx, 1] - 1)
        admittance_matrix[from_bus, to_bus] -= line_admittance[idx]
        admittance_matrix[to_bus, from_bus] = admittance_matrix[from_bus, to_bus]
        admittance_matrix[from_bus, from_bus] += line_admittance[idx] + line_susceptance[idx]
        admittance_matrix[to_bus, to_bus] += line_admittance[idx] + line_susceptance[idx]

    admittance_magnitude = np.abs(admittance_matrix)
    admittance_angle = np.angle(admittance_matrix)

    # Extract bus data
    bus_types = bus_parameters[:, 1]
    power_gen_real = bus_parameters[:, 2]
    power_gen_reactive = bus_parameters[:, 3]
    power_load_real = bus_parameters[:, 4]
    power_load_reactive = bus_parameters[:, 5]
    reactive_power_min = bus_parameters[:, 8]
    reactive_power_max = bus_parameters[:, 9]
    voltage_magnitude = bus_parameters[:, 6]
    voltage_angle = bus_parameters[:, 7]

    # Calculate net power injections
    net_power_real = power_gen_real - power_load_real
    net_power_reactive = power_gen_reactive - power_load_reactive

    # Initialize convergence parameters
    mismatch_tolerance = 0.001
    max_iterations = 10
    current_mismatch = 1
    iteration_count = 1

    # Main power flow iteration loop
    while current_mismatch >= mismatch_tolerance and iteration_count < max_iterations:
        power_calc_real = np.zeros(num_buses)
        power_calc_reactive = np.zeros(num_buses)

        # Calculate power injections
        for bus_i in range(1, num_buses):
            for bus_j in range(num_buses):
                angle_diff = admittance_angle[bus_i, bus_j] + voltage_angle[bus_j] - voltage_angle[bus_i]
                power_calc_real[bus_i] += voltage_magnitude[bus_i] * voltage_magnitude[bus_j] * \
                                        admittance_magnitude[bus_i, bus_j] * np.cos(angle_diff)
                power_calc_reactive[bus_i] -= voltage_magnitude[bus_i] * voltage_magnitude[bus_j] * \
                                            admittance_magnitude[bus_i, bus_j] * np.sin(angle_diff)

            # Check reactive power limits for PV buses
            if reactive_power_max[bus_i] != 0:
                if power_calc_reactive[bus_i] > reactive_power_max[bus_i]:
                    power_calc_reactive[bus_i] = reactive_power_max[bus_i]
                    bus_types[bus_i] = BUS_TYPE_PQ
                elif power_calc_reactive[bus_i] < reactive_power_min[bus_i]:
                    power_calc_reactive[bus_i] = reactive_power_min[bus_i]
                    bus_types[bus_i] = BUS_TYPE_PQ
                else:
                    bus_types[bus_i] = BUS_TYPE_PV
                    voltage_magnitude[bus_i] = bus_parameters[bus_i, 6]

        # Calculate power mismatches
        power_mismatch_real = net_power_real[1:] - power_calc_real[1:]
        pv_buses = bus_types == BUS_TYPE_PV
        power_mismatch_reactive = net_power_reactive[pv_buses] - power_calc_reactive[pv_buses]

        # Form Jacobian matrix components
        J1 = np.zeros((num_buses, num_buses))
        J2 = np.zeros((num_buses, num_buses))
        J3 = np.zeros((num_buses, num_buses))
        J4 = np.zeros((num_buses, num_buses))

        for bus_i in range(num_buses):
            for bus_j in range(num_buses):
                if bus_j != bus_i:
                    angle_diff = admittance_angle[bus_i, bus_j] + voltage_angle[bus_j] - voltage_angle[bus_i]
                    
                    J1[bus_i, bus_i] += voltage_magnitude[bus_i] * voltage_magnitude[bus_j] * \
                                       admittance_magnitude[bus_i, bus_j] * np.sin(angle_diff)
                    J1[bus_i, bus_j] = -voltage_magnitude[bus_i] * voltage_magnitude[bus_j] * \
                                      admittance_magnitude[bus_i, bus_j] * np.sin(angle_diff)

                    J2[bus_i, bus_i] += voltage_magnitude[bus_j] * admittance_magnitude[bus_i, bus_j] * \
                                       np.cos(angle_diff)
                    J2[bus_i, bus_j] = voltage_magnitude[bus_i] * admittance_magnitude[bus_i, bus_j] * \
                                      np.cos(angle_diff)

                    J3[bus_i, bus_i] += voltage_magnitude[bus_i] * voltage_magnitude[bus_j] * \
                                       admittance_magnitude[bus_i, bus_j] * np.cos(angle_diff)
                    J3[bus_i, bus_j] = -voltage_magnitude[bus_i] * voltage_magnitude[bus_j] * \
                                      admittance_magnitude[bus_i, bus_j] * np.cos(angle_diff)

            J4[bus_i, bus_i] = -2 * voltage_magnitude[bus_i] * admittance_magnitude[bus_i, bus_i] * \
                               np.sin(admittance_angle[bus_i, bus_i])

        # Extract relevant Jacobian submatrices
        non_slack_buses = bus_types != BUS_TYPE_SLACK
        J11 = J1[non_slack_buses][:, non_slack_buses]
        J22 = J2[non_slack_buses][:, pv_buses]
        J33 = J3[pv_buses][:, non_slack_buses]
        J44 = J4[pv_buses][:, pv_buses]

        # Form complete Jacobian and solve
        jacobian_matrix = np.block([
            [J11, J22],
            [J33, J44]
        ])

        mismatch_vector = np.concatenate([power_mismatch_real, power_mismatch_reactive])
        corrections = np.linalg.solve(jacobian_matrix, mismatch_vector)

        # Update state variables
        voltage_angle[non_slack_buses] += corrections[:len(power_mismatch_real)]
        voltage_magnitude[pv_buses] += corrections[len(power_mismatch_real):]

        current_mismatch = np.linalg.norm(mismatch_vector)
        iteration_count += 1

    # Calculate slack bus power
    for bus_j in range(num_buses):
        angle_diff = admittance_angle[0, bus_j] + voltage_angle[bus_j] - voltage_angle[0]
        power_gen_real[0] += voltage_magnitude[0] * voltage_magnitude[bus_j] * \
                            admittance_magnitude[0, bus_j] * np.cos(angle_diff)
        power_gen_reactive[0] -= voltage_magnitude[0] * voltage_magnitude[bus_j] * \
                                admittance_magnitude[0, bus_j] * np.sin(angle_diff)

    # Calculate reactive power for PQ buses
    pq_buses = np.where(bus_types == BUS_TYPE_PQ)[0]
    for bus_i in pq_buses:
        for bus_j in range(num_buses):
            angle_diff = admittance_angle[bus_i, bus_j] + voltage_angle[bus_j] - voltage_angle[bus_i]
            power_gen_reactive[bus_i] -= voltage_magnitude[bus_i] * voltage_magnitude[bus_j] * \
                                       admittance_magnitude[bus_i, bus_j] * np.sin(angle_diff)

    # Calculate system losses
    power_loss_real = np.sum(power_gen_real) - np.sum(power_load_real)
    power_loss_reactive = np.sum(power_gen_reactive) - np.sum(power_load_reactive)

    # Print results
    print(f"Total Iterations: {iteration_count}")
    print("\nVoltage Magnitude (p.u.):")
    print(voltage_magnitude)
    print("\nVoltage Angle (radians):")
    print(voltage_angle)
    print("\nVoltage Angle (degrees):")
    print(np.degrees(voltage_angle))
    print(f"\nTotal Real Power Loss (p.u.): {power_loss_real}")
    print(f"Total Reactive Power Loss (p.u.): {power_loss_reactive}")
    print(f"Total Real Power Loss (MW): {power_loss_real * system_base_mva}")
    print(f"Total Reactive Power Loss (MVAR): {power_loss_reactive * system_base_mva}")
    print(f"Active Power at Slack Bus (p.u.): {power_gen_real[0]}")
    print(f"Reactive Power at Slack Bus (p.u.): {power_gen_reactive[0]}")
    print(f"Active Power at Slack Bus (MW): {power_gen_real[0] * system_base_mva}")
    print(f"Reactive Power at Slack Bus (MVAR): {power_gen_reactive[0] * system_base_mva}")

    return {
        'voltage_magnitude': voltage_magnitude,
        'voltage_angle': voltage_angle,
        'power_loss_real': power_loss_real,
        'power_loss_reactive': power_loss_reactive,
        'slack_power_real': power_gen_real[0],
        'slack_power_reactive': power_gen_reactive[0],
        'iterations': iteration_count
    }

if __name__ == "__main__":
    results = calculate_power_flow()