# Managed State as a Pulley Backend

> *LDAP models with Managed State can trigger functionality
> at set times.  This is implemented as a Pulley Backend.
> Pulley is the SteamWorks component that subscribes to LDAP
> changes and processes them virtually immediately.  To get
> updates back into LDAP, the SteamWorks Crank component
> may be used.*

Managed state refers to an auxiliary LDAP object definition
`managedObject` which adds zero or more attributes
`managedState` that may hold named states, timer instructions
for entering such states and possible responses between the
attribute values.

The last entry in each managed state attribute is what matters
for processing.  It may setup a timer, or check if another
attribute reached a particular state, possibly due to a timer
expiring in that other attribute.


## Processing in the Pulley Backend

The internal state of the backend consists of a queue of
life cycles that await a timer should be fired in the future.
In addition, there is a queue of life cycles awaiting the
change of state in another life cycle.  When the queue
entries fire, they lead to an action that may change the
managed state.

A new `managedState` added to an LDAP object triggers the
pulley backend, so its new state may be added.  If an old
state is removed, this leads to the opposite.

Not all forms of `managedState` have an impact:

  * there should be a life cycle name and at least one
    attribute;
  * the last word should either have an `EV@TS` or `LC?EV`
    form;
  * in the `EV@TS` form, the timestamped event must not
    have fired yet;
  * in the `LC?EV` form, the referenced event must not
    have fired yet; it is *not* a constraint that `LC`
    must exist in the same object; if not, it will fire
    automatically (with a transaction as the decision
    boundary).

To be able to judge whether an event has fired, the
timestamp of the last ones to succeed are noticed.

Things may show up in a variety of orders, and actions
such as dismissing may be deferred, and so some care
must be taken to act properly, but the actual logic is
relatively simple.  And simple is good, when it does
not lack expressive power.


## Triggering Actions

The actions triggered are UNIX processes, and their
exit code counts to determine if an action succeeded.
*What to do with failure?*

The LDAP objects that are followed may be changed by
the actions.  This may well impact the life cycles,
by adding a new event or delay, and it may also have
an impact on other life cycles, though the `LC?EV`
form should capture most of that crossover knowledge.
More importantly, the other fields in the LDAP object
may be read and updated in line with the processing
requirements.  **TODO:** There may also be reasons
to tap into other objects, for instance to find a
link between private and public keys.

It is likely that libraries will follow a common
pattern:

 1. Retrieve the LDAP object
 2. Inspect state, decide what to do
 3. Take action
 4. Update the LDAP object

To allow this, the following information is supplied
to the action processor:

  * The DN of the LDAP object
  * The life cycle that triggers
  * The event that triggers (the last word)

To find the action, a configuration file or directory
structure will be traversed.  Every life cycle should
have its own handler.

One action can be started at a time, to ensure the
simplest possible handling.  The Crank can be used
to easily get and update LDAP objects; its HTTP or
FastCGI interface with JSON objects is easy to use.


## Idempotence

It is not a concern to repeat triggering events,
because the LDAP object can show that things have
changed.  This means that it is realistic to require
that actions are idempotent, even if this mechanism
is the way it is done.  Note that an event name
MUST be unique for a given life cycle in a given
LDAP object.

The result is that we can assure that events will
always be triggered; the only requirement is that
we send events at least often enough.  So, when a
timer expires, its timestamp (or even one less)
can be stored as finished, but only after having
finished all the work for that timestamp.  Lagging
behind in work-done administration is the safe
choice.


## Finally Done

When a process is finished, it is not marked with
an end marker like a dot.  That would work, but at
restart of the system the entire state would need
to flush through the event system, potentially
leading to quadratic delays on large data sets.

Instead, we can remove the `managedState` attribute.
However, for historic purposes, we might call it
the `managedHistory` attribute instead.  The
event-drive structures ignore those attributes
completely, even for `ID?EV` they are gone.  At
any time at the administrator's convenience, the
`managedHistory` logs may be removed.  In most
cases however, it would be expected of some use.

