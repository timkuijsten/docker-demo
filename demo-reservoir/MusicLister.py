#!/usr/bin/env python3
#
# MusicLister.py / .txt -- List music and play it
#
# This is a minimal Reservoir application, based on the API, and it
# has served as a learning experience.  Having said that, it is also
# a good demonstration of the interaction between a web-frontend and
# a Reservoir backend.
#
# This was developed together with the Docker image arpa2/files-reservoir
# containing an LDIF and a matching /var/arpa2/reservoir tree that was
# manually constructed as bootstrapping data.  This image distributes
# a wonderful album "Whispering" by "Negrin", who kindly made it available
# under a Creative Commons License.  Thank you, Negrin :) the music is an
# interesting surprise to run into after setting this up.
#
# By Henri Manson (with a bit of help by Rick)

from twisted.internet import reactor
from twisted.web.resource import Resource
from twisted.web.server import Site, NOT_DONE_YET
from twisted.web.template import Element, renderer, XMLFile, flattenString
from twisted.python.filepath import FilePath

from arpa2 import reservoir

class WidgetsElement(Element):
    loader = XMLFile(FilePath('MusicLister.xml'))

    @renderer
    def widgets(self, request, tag):
        for widget in self.widgetData:
            yield tag.clone().fillSlots(href=widget['href'], content=widget['content'])

class MusicList(Resource):
    isLeaf = True

    def printResult(self, result):
        self.request.write(result)
        self.request.finish()

    def render_GET(self, request):
        try:
            (clx,resource) = reservoir.uri_canonical_open(request.uri.decode('utf-8'))

            if resource is None:
                w = WidgetsElement()
                w.widgetData = [ ]
                for item in clx.items():
                    entry = { 'href': '../' + item[1] + '/', 'content': item[0] }
                    w.widgetData.append(entry)
                resources = clx.load_all_resources()
                for resource in resources:
                    result = ""
                    for cn in resource['cn']:
                        result += cn
                    entry = { 'href': resource.resuuid, 'content': result }
                    w.widgetData.append(entry)
                d = flattenString(None, w)
                self.request = request
                d.addCallback(self.printResult)
                return NOT_DONE_YET
            else:
                fh = resource.open(binary=True)
                fileContent = fh.read()
                fh.close()
                for media_type in resource['mediaType']:
                    request.setHeader('Content-type', media_type)
                return fileContent
        except:
            (uri,clx,resource) = reservoir.uri_canonical(domain='arpa2.org', apphint='Music', domain_relative=True)
            host = request.getHeader('host')
            request.redirect(('//' + host + uri).encode('utf-8'))
            request.finish()
            return NOT_DONE_YET

factory = Site(MusicList())
reactor.listenTCP(8811, factory)
reactor.run()
