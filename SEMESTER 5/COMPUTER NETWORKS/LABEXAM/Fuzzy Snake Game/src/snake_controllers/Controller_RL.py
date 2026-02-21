import pygame
import numpy as np
import random
from collections import deque
import pickle
import math
from queue import PriorityQueue

class SnakeGame:
    def __init__(self, width=400, height=400, grid_size=20):
        self.width = width
        self.height = height
        self.grid_size = grid_size
        self.max_steps = (width * height) // (grid_size * grid_size)
        self.reset()
        
        pygame.init()
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption('Snake Game')
        self.clock = pygame.time.Clock()
        
    def reset(self):
        self.snake = deque([(self.width//(2*self.grid_size) * self.grid_size, 
                           self.height//(2*self.grid_size) * self.grid_size)])
        self.direction = (self.grid_size, 0)  # Start moving right
        self.score = 0
        self.steps_without_food = 0
        self.place_food()
        self.game_over = False
        return self.get_state()
    
    def place_food(self):
        while True:
            self.food = (random.randint(0, (self.width//self.grid_size)-1) * self.grid_size,
                        random.randint(0, (self.height//self.grid_size)-1) * self.grid_size)
            if self.food not in self.snake:
                break

    def get_state(self):
        head = self.snake[0]
        
        # Current direction one-hot encoding
        dir_state = [0, 0, 0, 0]  # [left, right, up, down]
        if self.direction == (-self.grid_size, 0): dir_state[0] = 1
        elif self.direction == (self.grid_size, 0): dir_state[1] = 1
        elif self.direction == (0, -self.grid_size): dir_state[2] = 1
        elif self.direction == (0, self.grid_size): dir_state[3] = 1
        
        # Dangers in all directions
        dangers = self.get_dangers()
        
        # Food direction relative to head
        food_dir = [
            int(self.food[0] < head[0]),  # food left
            int(self.food[0] > head[0]),  # food right
            int(self.food[1] < head[1]),  # food up
            int(self.food[1] > head[1])   # food down
        ]
        
        return np.array(dangers + dir_state + food_dir, dtype=int)

    def get_dangers(self):
        head = self.snake[0]
        points = [
            (head[0] - self.grid_size, head[1]),  # left
            (head[0] + self.grid_size, head[1]),  # right
            (head[0], head[1] - self.grid_size),  # up
            (head[0], head[1] + self.grid_size)   # down
        ]
        return [int(self.is_collision(p)) for p in points]

    def is_collision(self, point):
        return (point[0] < 0 or point[0] >= self.width or 
                point[1] < 0 or point[1] >= self.height or 
                point in list(self.snake)[1:])
    
    def get_available_neighbors(self, pos):
        neighbors = []
        for dx, dy in [(0, -self.grid_size), (0, self.grid_size), 
                      (-self.grid_size, 0), (self.grid_size, 0)]:
            new_pos = (pos[0] + dx, pos[1] + dy)
            if not self.is_collision(new_pos):
                neighbors.append(new_pos)
        return neighbors

    def find_path_to_food(self):
        start = self.snake[0]
        goal = self.food
        
        frontier = PriorityQueue()
        frontier.put((0, start))
        came_from = {start: None}
        cost_so_far = {start: 0}
        
        while not frontier.empty():
            current = frontier.get()[1]
            
            if current == goal:
                break
                
            for next_pos in self.get_available_neighbors(current):
                new_cost = cost_so_far[current] + 1
                if next_pos not in cost_so_far or new_cost < cost_so_far[next_pos]:
                    cost_so_far[next_pos] = new_cost
                    priority = new_cost + self.manhattan_distance(next_pos, goal)
                    frontier.put((priority, next_pos))
                    came_from[next_pos] = current
        
        if goal not in came_from:
            return None
            
        path = []
        current = goal
        while current != start:
            path.append(current)
            current = came_from[current]
        path.append(start)
        path.reverse()
        return path

    def manhattan_distance(self, p1, p2):
        return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])

    def get_action_for_next_position(self, next_pos):
        head = self.snake[0]
        new_direction = (next_pos[0] - head[0], next_pos[1] - head[1])
        
        # Convert direction change to action (0: straight, 1: right, 2: left)
        current_direction_idx = [(0, -self.grid_size), (self.grid_size, 0), 
                               (0, self.grid_size), (-self.grid_size, 0)].index(self.direction)
        new_direction_idx = [(0, -self.grid_size), (self.grid_size, 0), 
                           (0, self.grid_size), (-self.grid_size, 0)].index(new_direction)
        
        # Calculate the number of right turns needed
        turns_needed = (new_direction_idx - current_direction_idx) % 4
        if turns_needed == 0:
            return 0  # straight
        elif turns_needed == 1:
            return 1  # right turn
        else:
            return 2  # left turn

    def step(self, action):
        self.steps_without_food += 1
        
        # [straight, right, left]
        clock_wise = [(0, -self.grid_size), (self.grid_size, 0), 
                     (0, self.grid_size), (-self.grid_size, 0)]
        idx = clock_wise.index(self.direction)
        
        if action == 0:  # straight
            new_dir = clock_wise[idx]
        elif action == 1:  # right turn
            new_dir = clock_wise[(idx + 1) % 4]
        else:  # left turn
            new_dir = clock_wise[(idx - 1) % 4]
            
        self.direction = new_dir
        head = self.snake[0]
        new_head = (head[0] + self.direction[0], head[1] + self.direction[1])
        
        reward = 0
        if self.is_collision(new_head):
            self.game_over = True
            reward = -10
        else:
            self.snake.appendleft(new_head)
            if new_head == self.food:
                self.score += 1
                reward = 10
                self.steps_without_food = 0
                self.place_food()
            else:
                self.snake.pop()
                
            if self.steps_without_food > self.max_steps:
                self.game_over = True
                reward = -10
                
        return self.get_state(), reward, self.game_over, self.score

    def render(self):
        self.screen.fill((0, 0, 0))
        
        # Draw snake
        for i, segment in enumerate(self.snake):
            color = (0, 255, 0) if i == 0 else (0, 200, 0)  # Head is brighter
            pygame.draw.rect(self.screen, color, 
                           pygame.Rect(segment[0], segment[1], 
                                     self.grid_size-2, self.grid_size-2))
        
        # Draw food
        pygame.draw.rect(self.screen, (255, 0, 0), 
                        pygame.Rect(self.food[0], self.food[1], 
                                  self.grid_size-2, self.grid_size-2))
        
        # Draw score
        font = pygame.font.Font(None, 36)
        score_text = font.render(f'Score: {self.score}', True, (255, 255, 255))
        self.screen.blit(score_text, (10, 10))
        
        pygame.display.flip()
        self.clock.tick(10)

class HybridAgent:
    def __init__(self, state_size=12, action_size=3):
        self.state_size = state_size
        self.action_size = action_size
        self.q_table = self.initialize_q_table()
        
    def initialize_q_table(self):
        """Initialize Q-table with pre-trained values"""
        # This is a simplified pre-trained Q-table
        q_table = {}
        # Add some basic pre-trained values
        for i in range(2**self.state_size):
            state = tuple(int(x) for x in format(i, f'0{self.state_size}b'))
            q_table[state] = np.array([1.0, 1.0, 1.0])  # Initialize with optimistic values
        return q_table
    
    def get_action(self, state, game):
        # Try to find a path to food first
        path = game.find_path_to_food()
        if path and len(path) > 1:
            return game.get_action_for_next_position(path[1])
        
        # If no path found, use Q-table
        state_tuple = tuple(state)
        if state_tuple not in self.q_table:
            self.q_table[state_tuple] = np.array([1.0, 1.0, 1.0])
        return np.argmax(self.q_table[state_tuple])

def RLController():
    env = SnakeGame()
    agent = HybridAgent()
    
    while True:
        state = env.reset()
        total_reward = 0
        
        while not env.game_over:
            action = agent.get_action(state, env)
            next_state, reward, done, score = env.step(action)
            state = next_state
            total_reward += reward
            
            env.render()
            
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    return
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q:
                        pygame.quit()
                        return
        
        print(f'Game Over! Score: {score}')

if __name__ == "__main__":
    RLController()