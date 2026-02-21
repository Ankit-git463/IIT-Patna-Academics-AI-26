start([3,3,left,0,0]).
goal([0,0,right,3,3]).

valid(CL,ML,CR,MR) :-
    % Check if the current state is valid
    ML>=0, CL>=0, MR>=0, CR>=0,
    (ML>=CL ; ML=0),
    (MR>=CR ; MR=0).

% Possible actions
move_two_missionaries_left_to_right([CL,ML,left,CR,MR],[CL,ML2,right,CR,MR2]):-
    % Two missionaries cross from left to right
    MR2 is MR+2,
    ML2 is ML-2,
    valid(CL,ML2,CR,MR2).

move_two_cannibals_left_to_right([CL,ML,left,CR,MR],[CL2,ML,right,CR2,MR]):-
    % Two cannibals cross from left to right
    CR2 is CR+2,
    CL2 is CL-2,
    valid(CL2,ML,CR2,MR).

move_one_missionary_one_cannibal_left_to_right([CL,ML,left,CR,MR],[CL2,ML2,right,CR2,MR2]):-
    % One missionary and one cannibal cross from left to right
    CR2 is CR+1,
    CL2 is CL-1,
    MR2 is MR+1,
    ML2 is ML-1,
    valid(CL2,ML2,CR2,MR2).

move_one_missionary_left_to_right([CL,ML,left,CR,MR],[CL,ML2,right,CR,MR2]):-
    % One missionary crosses from left to right
    MR2 is MR+1,
    ML2 is ML-1,
    valid(CL,ML2,CR,MR2).

move_one_cannibal_left_to_right([CL,ML,left,CR,MR],[CL2,ML,right,CR2,MR]):-
    % One cannibal crosses from left to right
    CR2 is CR+1,
    CL2 is CL-1,
    valid(CL2,ML,CR2,MR).

move_two_missionaries_right_to_left([CL,ML,right,CR,MR],[CL,ML2,left,CR,MR2]):-
    % Two missionaries cross from right to left
    MR2 is MR-2,
    ML2 is ML+2,
    valid(CL,ML2,CR,MR2).

move_two_cannibals_right_to_left([CL,ML,right,CR,MR],[CL2,ML,left,CR2,MR]):-
    % Two cannibals cross from right to left
    CR2 is CR-2,
    CL2 is CL+2,
    valid(CL2,ML,CR2,MR).

move_one_missionary_one_cannibal_right_to_left([CL,ML,right,CR,MR],[CL2,ML2,left,CR2,MR2]):-
    % One missionary and one cannibal cross from right to left
    CR2 is CR-1,
    CL2 is CL+1,
    MR2 is MR-1,
    ML2 is ML+1,
    valid(CL2,ML2,CR2,MR2).

move_one_missionary_right_to_left([CL,ML,right,CR,MR],[CL,ML2,left,CR,MR2]):-
    % One missionary crosses from right to left
    MR2 is MR-1,
    ML2 is ML+1,
    valid(CL,ML2,CR,MR2).

move_one_cannibal_right_to_left([CL,ML,right,CR,MR],[CL2,ML,left,CR2,MR]):-
    % One cannibal crosses from right to left
    CR2 is CR-1,
    CL2 is CL+1,
    valid(CL2,ML,CR2,MR).

% Recursive call to solve the problem
solve_problem([CL1,ML1,B1,CR1,MR1],[CL2,ML2,B2,CR2,MR2],Explored,MovesList) :-
    % Attempt to make a valid move and recursively call the solver
    (
        move_two_missionaries_left_to_right([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_two_cannibals_left_to_right([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_one_missionary_one_cannibal_left_to_right([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_one_missionary_left_to_right([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_one_cannibal_left_to_right([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_two_missionaries_right_to_left([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_two_cannibals_right_to_left([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_one_missionary_one_cannibal_right_to_left([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_one_missionary_right_to_left([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3]);
        move_one_cannibal_right_to_left([CL1,ML1,B1,CR1,MR1],[CL3,ML3,B3,CR3,MR3])
    ),
    not(member([CL3,ML3,B3,CR3,MR3],Explored)),
    solve_problem([CL3,ML3,B3,CR3,MR3],[CL2,ML2,B2,CR2,MR2],[[CL3,ML3,B3,CR3,MR3]|Explored],[[[CL3,ML3,B3,CR3,MR3],[CL1,ML1,B1,CR1,MR1]]|MovesList]).

% Solution found
solve_problem([CL,ML,B,CR,MR],[CL,ML,B,CR,MR],_,MovesList):-
    print_solution(MovesList).

% Print the solution
print_solution([]) :- nl.
print_solution([[A,B]|MovesList]) :-
    print_solution(MovesList),
    write(B), write(' -> '), write(A), nl.

% Find the solution for the missionaries and cannibals problem
find_solution :-
    solve_problem([3,3,left,0,0],[0,0,right,3,3],[[3,3,left,0,0]],_).