class Path:
    def __init__(self, base, args):
        self.base = base
        self.args = args

    def env(self):
        return "/".join([self.base, ".tec/toggles" ])

    def desk(self):
        return "/".join([self.base, self.args['env'], ".tec/toggles"])

    def task(self):
        return "/".join([self.base, self.args['env'], self.args['desk'], ".tec/toggles"])

class Togg:
    def __init__(self, base, args):
        self.base = base
        self.args = args
        self.path = Path(base, args)

    def _parse(self, fname):
        toggs = {}
        with open(fname) as f:
            for line in f:
                line = line.strip()
                key, val = line.split(" : ")
                toggs[key] = val
        return toggs

    def gettask(self):
        fname = self.path.task()
        return self._parse(fname)

    def getdesk(self):
        fname = self.path.desk()
        return self._parse(fname)

    def getenv(self):
        fname = self.path.env()
        return self._parse(fname)
