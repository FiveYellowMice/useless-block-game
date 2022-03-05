% Simulation parameter
deltaFrameTime = 1/60; % fps^-1
simDuration = 6; % s
nFrames = simDuration / deltaFrameTime;

t = 0 : deltaFrameTime : simDuration-deltaFrameTime;
targetVelocity = nan(nFrames, 1);
acceleration = zeros(nFrames, 1);
velocity = zeros(nFrames, 1);
position = zeros(nFrames, 1);

% Initial condition
velocity(1) = 0; % m/s
position(1) = 0; % m

% Start walking at 0.5s, stop at 4s
targetVelocity(0.5/deltaFrameTime+1 : 4/deltaFrameTime) = 5;
targetVelocity(4/deltaFrameTime+1 : 6/deltaFrameTime) = 0;

for i = 2 : nFrames
  % Try to reach target velocity
  if abs(targetVelocity(i-1) - velocity(i-1)) > 0.001
    acceleration(i) += 2/pi * atan(4 * (targetVelocity(i-1) - velocity(i-1))) * 8;
  end
  
  % Friction/Drag
  acceleration(i) -= 2/pi * atan(velocity(i-1)) * (2.1 + velocity(i-1)^2 * 0.01);
  
  velocity(i) = velocity(i-1) + acceleration(i) * deltaFrameTime;
  position(i) = position(i-1) + velocity(i) * deltaFrameTime;
end

plot(t, targetVelocity, 'Color', 'black', 'DisplayName', "Target Velocity");
hold on;
plot(t, acceleration, 'r', 'DisplayName', "Acceleration");
plot(t, velocity, 'g', 'DisplayName', "Velocity");
plot(t, position, 'b', 'DisplayName', "Position");
hold off;
legend('Location', 'northeast');
