package cz.cvut.fit.tjv.realestates.client.ui;

import cz.cvut.fit.tjv.realestates.client.data.OwnerClient;
import cz.cvut.fit.tjv.realestates.client.model.OwnerDTO;
import cz.cvut.fit.tjv.realestates.client.model.PropertyDTO;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@Controller
@RequestMapping("/rest/api/owner")
public class OwnerWebController {
    private final OwnerClient ownerClient;

    public OwnerWebController(OwnerClient ownerClient) {
        this.ownerClient = ownerClient;
    }

    @GetMapping("/{id}")
    public String getOwner(@PathVariable("id") String email, Model model) {
        model.addAttribute("newProperty", new PropertyDTO());
        model.addAttribute("owner", ownerClient.getOwnerById(email));
        return "Owner/OwnerByEmail";
    }

    @GetMapping
    public String getOwners(Model model) {
        model.addAttribute("newOwner", new OwnerDTO());
        model.addAttribute("owners", ownerClient.getAllOwners());
        return "Owner/Owners";
    }

    @PostMapping
    public String addOwner(@ModelAttribute OwnerDTO ownerDTO, Model model) {
        model.addAttribute(ownerClient.createOwner(ownerDTO));
        return "redirect:/rest/api/owner";
    }

    @PostMapping("/{id}/update")
    public String updateOwner(@PathVariable("id") String email, @ModelAttribute OwnerDTO ownerDTO, Model model) {
        model.addAttribute(ownerClient.updateOwner(ownerDTO, email));
        return "redirect:/rest/api/owner";
    }

    @PostMapping("/{id}")
    public String deleteOwner(@PathVariable("id") String email, Model model) {
        model.addAttribute(ownerClient.deleteOwnerById(email));
        return "redirect:/rest/api/owner";
    }

    @PostMapping("/{id}/properties")
    public String addOwnerToProperty(@PathVariable("id") String email, @ModelAttribute PropertyDTO propertyDTO, Model model) {
        model.addAttribute("addOwnerToProperty", ownerClient.addOwnerToProperty(email, propertyDTO));
        return "redirect:/rest/api/owner/" + email;
    }

    @PostMapping("/{id}/properties/{id_property}")
    public String deleteOwnerFromProperty(@PathVariable("id") String email, @PathVariable("id_property") Long propertyID, Model model) {
        model.addAttribute("removeOwnerFromProperty", ownerClient.removeOwnerFromProperty(email, propertyID));
        return "redirect:/rest/api/owner/" + email;
    }
}
