listener->onTouchBegan = [=](Touch* touch, Event* e) {
        points.clear();
        points.push_back(touch->getLocation());
        
        outline1.clear();
        outline2.clear();

        return true;
    };
    
    float lineWidth = 10.0f;
    
    
    listener->onTouchMoved = [=](Touch* touch, Event* e) {
        auto lineStart = points.at(points.size() - 1);
        auto lineEnd = lineStart;
        lineEnd.smooth(touch->getLocation(), 0.1, 1);
        lineEnd = touch->getLocation();
        if (points.size() == 1) {
            auto angle = atan2f(lineEnd.y - lineStart.y, lineEnd.x - lineEnd.x);
            outline1.push_back(Point(lineStart.x + sinf(angle) * lineWidth, lineStart.y - cosf(angle) * lineWidth));
            outline2.push_back(Point(lineStart.x - sinf(angle) * lineWidth, lineStart.y + cosf(angle) * lineWidth));
        } else {
            auto prevPoint = points.at(points.size() - 2);
            float angle1;
            if (lineStart.y == prevPoint.y) {
                angle1 = (lineStart.x < prevPoint.x)? M_PI : 0;
            } else {
                angle1 = atan2f(lineStart.y - prevPoint.y, lineStart.x - prevPoint.x);
            }
            float angle2;
            if (lineEnd.y == lineStart.y) {
                angle2 = (lineEnd.x < lineStart.x)? M_PI : 0;
            } else {
                angle2 = atan2f(lineEnd.y - lineStart.y, lineEnd.x - lineStart.x);
            }
            auto angle = (angle1 + angle2)/2 + M_PI_2;
            auto s = sinf(angle1 - angle);
            auto h = (s < 0.5)? lineWidth : abs(lineWidth/s);
            
            auto p1 = Point(lineEnd.x - h * cosf(angle), lineEnd.y - sinf(angle) * h);
            auto p2 = Point(lineEnd.x + h * cosf(angle), lineEnd.y + sinf(angle) * h);
            
            Vec2 v1 = Vec2(p1.x - lineStart.x, p1.y - lineStart.y);
            Vec2 v2 = Vec2(p2.x - lineStart.x, p2.y - lineStart.y);
            float z = v1.x * v2.y - v1.y * v2.x;
            
            if (z < 0) {
                outline1.push_back(p2);
                outline2.push_back(p1);
            } else {
                outline1.push_back(p1);
                outline2.push_back(p2);
            }
        }
        points.push_back(touch->getLocation());
    };
    
    listener->onTouchEnded = [=](Touch* touch, Event* e) {
        //add the last points
    };
