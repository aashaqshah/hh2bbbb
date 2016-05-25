
from ROOT import TChain, TEventList
from math import floor
import numpy as np


class EventListCreator:
    ''' Produces a TEventList to select subsets of a TChain '''

    sam_names = []
    sam_events = []
    aim_events = []

    def __init__(self, lumi = 1.0, l_name ="subset", t_name = "tree"):
        self.lumi = lumi 
        self.l_name = l_name # list name
        self.t_name = t_name # tree name

    def add_sample(self, sam_name, xs = -1, gen_event = -1,
                   factor = 1.0, aim_event = -1):
        self.sam_names.append(sam_name)
        # get number of entries in sample (regexp)
        tc = TChain(self.t_name)
        tc.Add(sam_name)
        sam_event = tc.GetEntries()
        self.sam_events.append(sam_event)
        if aim_event > 0: 
            self.aim_events.append(aim_event)
        elif (xs > 0) and (gen_event > 0):
            self.aim_events.append(factor*self.lumi*xs*sam_event/float(gen_event))
        else:
            self.aim_events.append(0)

    def get_event_list(self):
        el = TEventList(self.l_name)
        # ratio of events available to the amound required
        rat_events = [sam/float(aim) for sam, aim in zip(self.sam_events,
                                                         self.aim_events)]
        min_rat = min(rat_events)
        # reescale lumi and aim events if not enough events in samples
        if (min_rat < 1.):
            self.lumi *= min_rat
            self.aim_events = [int(floor(aim*min_rat)) for aim in self.aim_events]
            print "Aim luminosity had to be rescaled to {} fb-1".format(self.lumi)
        # enter event numbers in entry list 
        tc_ev = 0
        for sam, aim in zip(self.sam_events, self.aim_events):
            # select aim first events
            sel_evs = np.arange(tc_ev, tc_ev+aim)
            tc_ev += sam # update initial event number 
            for sel_ev in sel_evs:
                el.Enter(sel_ev)
        return el        
