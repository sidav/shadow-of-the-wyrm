#pragma once
#include <boost/shared_ptr.hpp>

class Creature;

// StatusChange is a base class for dealing with status changes to
// creatures.  It records the start time and duration of the status
// change, and methods to apply, finalize, and undo the change, as well
// as a method to do something every minute along the way, if required.
class StatusChange
{
  public:
    StatusChange();
    virtual ~StatusChange();

    // Methods to handle creating, updating, removing, etc., the status change
    virtual void apply_change(boost::shared_ptr<Creature> creature) const;
    virtual void finalize_change(boost::shared_ptr<Creature> creature) const;
    virtual void undo_change(boost::shared_ptr<Creature> creature) const;

    // By default, nothing happens each tick.  Subclasses should override
    // this behaviour as necessary.
    virtual void tick(boost::shared_ptr<Creature> creature) const;
};

typedef boost::shared_ptr<StatusChange> StatusChangePtr;

