#pragma once

class ActivityTracker
{
private:
    double lastActivityTime;
    
    bool detect_activity() const;
    
public:
    ActivityTracker();
    
    void update();
    float seconds_since_activity() const;
};
